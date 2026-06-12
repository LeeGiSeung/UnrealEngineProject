// Fill out your copyright notice in the Description page of Project Settings.


#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "City/Building/BuildingBase/ABuildingBase.h"
#include "City/DataAsset/BuildingDataAsset.h"
#include "City/RoadActor/RoadActor.h"
#include <algorithm>
#include "Kismet/GameplayStatics.h"
#include "ProjectCharacter.h"
#include "Containers/Queue.h"

#pragma execution_character_set("utf-8")
using namespace std;


bool sortBuildingData(const FBuildingData& A, const FBuildingData& B) {
	double DistA = (A.CenterLocation.X * A.CenterLocation.X) + (A.CenterLocation.Y * A.CenterLocation.Y);

	double DistB = (B.CenterLocation.X * B.CenterLocation.X) + (B.CenterLocation.Y * B.CenterLocation.Y);

	return DistA < DistB;
}

bool sortroad(const FRoadData& A, const FRoadData& B) {
	double DistA = (A.Points[0].X * A.Points[0].X) + (A.Points[0].Y * A.Points[0].Y);

	double DistB = (B.Points[0].X * B.Points[0].X) + (B.Points[0].Y * B.Points[0].Y);

	return DistA < DistB;
}

void UUCityNewworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bool buildingFlag;
	LoadBuildingDataAsset(buildingFlag);
	if (buildingFlag) return;
	
	LoadQGIS();
}

void UUCityNewworkManager::LoadBuildingDataAsset(bool& retFlag)
{
	retFlag = true;
	UWorld* World = GetWorld();
	if (!World) return;

	FString AssetPath = TEXT("/Game/BluePrint/City/DataAsset/BP_BuildingDataAsset.BP_BuildingDataAsset_C");
	UClass* AssetClass = StaticLoadClass(UObject::StaticClass(), nullptr, *AssetPath);

	if (AssetClass)
	{
		UBuildingDataAsset* LoadedConfig = Cast<UBuildingDataAsset>(AssetClass->GetDefaultObject());
		BuildingBase = LoadedConfig->BuildingBase;
		BuildingBetweenDistance = LoadedConfig->BuildingBetweenDistance;
		RoadActorClass = LoadedConfig->RoadActor;
		DebugBlockClass = LoadedConfig->DebugBlockClass;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NO ASSETCLASS"));
	}
	retFlag = false;
}

TArray<FRoadNode> UUCityNewworkManager::GetNavigationCourse()
{
	return NavigationCourse;
}

void UUCityNewworkManager::LoadQGIS()
{
	//################################################
	//빌딩 데이터 Load
	//################################################
	//bool BuildingFlag;
	//LoadBuilding(BuildingFlag);
	//if (BuildingFlag) return;

	//################################################
	//도로 데이터 Load
	//################################################
	bool RoadFlag;
	LoadRoad(RoadFlag);
	if (RoadFlag) return;

	
}


void UUCityNewworkManager::LoadRoad(bool& retFlag)
{
	retFlag = true;
	UWorld* world = GetWorld();
	if (!world) {
		UE_LOG(LogTemp, Error, TEXT("NO WORLD OR BASE"));
		return;
	}

	FString RelativePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("IncheonLandFile/IncheonData/RoadIncheon.geojson"));
	FString DataPath = FPaths::ConvertRelativePathToFull(RelativePath);

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *DataPath)) {
		UE_LOG(LogTemp, Error, TEXT("NO FILE"));
		return;
	}

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject;

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid()) {
		UE_LOG(LogTemp, Error, TEXT("Fail string to json"));
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* FeaturesArray;
	if (!JsonObject->TryGetArrayField(TEXT("features"), FeaturesArray)) return;

	double minx = TNumericLimits<double>::Max();
	double miny = TNumericLimits<double>::Max();

	for (const auto& FeatureValue : *FeaturesArray) {
		TSharedPtr<FJsonObject> FeatureObj = FeatureValue->AsObject();
		if (!FeatureObj.IsValid()) continue;

		TSharedPtr<FJsonObject> GeometryObj = FeatureObj->GetObjectField(TEXT("geometry"));
		if (!GeometryObj.IsValid()) continue;

		FString GeoType;
		if (!GeometryObj->TryGetStringField(TEXT("type"), GeoType) || GeoType != TEXT("MultiLineString")) continue;

		const TArray<TSharedPtr<FJsonValue>>* Coordinates = nullptr;
		if (GeometryObj->TryGetArrayField(TEXT("coordinates"), Coordinates) && Coordinates->Num() > 0) {

			const TArray<TSharedPtr<FJsonValue>>& PointsArray = (*Coordinates)[0]->AsArray();

			for (const auto& PointValue : PointsArray) {
				const TArray<TSharedPtr<FJsonValue>>& Pt = PointValue->AsArray();
				if (Pt.Num() >= 2) {
					double X = Pt[0]->AsNumber();
					double Y = Pt[1]->AsNumber();

					if (X < minx) minx = X;
					if (Y < miny) miny = Y;
				}
			}
		}
	}

	if (minx == TNumericLimits<double>::Max() || miny == TNumericLimits<double>::Max()) {
		UE_LOG(LogTemp, Error, TEXT("No valid coordinates found."));
		return;
	}

	TArray<FRoadData> RoadDataList;

	// === 2. 실제 데이터 파싱 ===
	for (const auto& FeatureValue : *FeaturesArray) {
		TSharedPtr<FJsonObject> FeatureObj = FeatureValue->AsObject();
		if (!FeatureObj.IsValid()) continue;

		TSharedPtr<FJsonObject> GeometryObj = FeatureObj->GetObjectField(TEXT("geometry"));
		if (!GeometryObj.IsValid()) continue;

		FString GeoType;
		if (!GeometryObj->TryGetStringField(TEXT("type"), GeoType) || GeoType != TEXT("MultiLineString")) continue;

		FRoadData rData;

		TSharedPtr<FJsonObject> PropertiesObj = FeatureObj->GetObjectField(TEXT("properties"));

		if (PropertiesObj.IsValid()) {

			// 한글 키값 적용 ("차로수")
			rData.RoadCount = PropertiesObj->HasField(TEXT("RoadCount")) ? PropertiesObj->GetIntegerField(TEXT("RoadCount")) : 1;

			// 한글 키값 적용 ("도로폭" - 소수점이므로 GetNumberField 사용)
			rData.RoadWidth = PropertiesObj->HasField(TEXT("RoadWidth")) ? PropertiesObj->GetNumberField(TEXT("RoadWidth")) : 1.0f;
		}

			const TArray<TSharedPtr<FJsonValue>>* Coordinates = nullptr;
			if (GeometryObj->TryGetArrayField(TEXT("coordinates"), Coordinates) && Coordinates->Num() > 0) {

				const TArray<TSharedPtr<FJsonValue>>& PointsArray = (*Coordinates)[0]->AsArray();

				for (const auto& PointValue : PointsArray) {
					const TArray<TSharedPtr<FJsonValue>>& Pt = PointValue->AsArray();
					if (Pt.Num() >= 2) {
						double RawX = Pt[0]->AsNumber();
						double RawY = Pt[1]->AsNumber();

						float LocalX = (RawX - minx) * BuildingBetweenDistance;
						float LocalY = -((RawY - miny) * BuildingBetweenDistance);

						rData.Points.Add(FVector(LocalX, LocalY, 0.0f));
					}
				}

				if (rData.Points.Num() > 1) {
					RoadDataList.Add(rData);
				}
			}
		}

		RoadDataList.Sort(sortroad);

		for (int i = 0; i < RoadDataList.Num(); i++) { //테스트
			const FRoadData& RoadData = RoadDataList[i];

			FVector RoadSpawnLocation = RoadData.Points[0];
			FRotator Roadrotator = FRotator::ZeroRotator;

			if (RoadActorClass) {
				ARoadActor* roadactor = world->SpawnActor<ARoadActor>(RoadActorClass, RoadSpawnLocation, Roadrotator);
				if (roadactor) {

					TArray<FVector> LocalPoints;
					for (const FVector& GlobalPt : RoadData.Points) {

						LocalPoints.Add(GlobalPt - RoadSpawnLocation);
					}

					roadactor->SpawnRoadActor(LocalPoints, RoadData.RoadCount, RoadData.RoadWidth);

					roadactor->SetWorldPoints(RoadData.Points);
				}

				OutRoadVector.Add(roadactor);
			}

	
		}

	BuildNavigationNetwork();
	
	retFlag = false;
}

void UUCityNewworkManager::LoadBuilding(bool& retFlag)
{
	retFlag = true;
	UWorld* world = GetWorld();
	if (!BuildingBase || !world) {
		UE_LOG(LogTemp, Error, TEXT("NO BUILDING BASE"));
		return;
	}

	FString RelativePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("IncheonLandFile/IncheonData/IncheonJson.geojson"));

	FString DataPath = FPaths::ConvertRelativePathToFull(RelativePath);

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *DataPath)) {
		UE_LOG(LogTemp, Error, TEXT("NO FILE"));
		return;
	}

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject;
	TArray<FBuildingData> BuildingData;

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid()) {
		UE_LOG(LogTemp, Error, TEXT("Fail string to json"));
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* FeaturesArray;
	if (!JsonObject->TryGetArrayField(TEXT("features"), FeaturesArray)) return;

	int minx = 1e9;
	int miny = 1e9;

	for (const auto& FeatureValue : *FeaturesArray) {
		TSharedPtr<FJsonObject> FeatureObj = FeatureValue->AsObject();
		if (!FeatureObj.IsValid()) continue;

		TSharedPtr<FJsonObject> GeometryObj = FeatureObj->GetObjectField(TEXT("geometry"));
		if (!GeometryObj.IsValid()) continue;

		const TArray<TSharedPtr<FJsonValue>>* Coordinates = nullptr;

		if (GeometryObj->TryGetArrayField(TEXT("coordinates"), Coordinates) && Coordinates->Num() > 0) {

			const TArray<TSharedPtr<FJsonValue>>& Ring = (*Coordinates)[0]->AsArray();

			if (Ring.Num() > 0) {
				const TArray<TSharedPtr<FJsonValue>>& Points = Ring[0]->AsArray();

				// 3. 내부 루프를 돌며 각 좌표쌍 추출
				for (const auto& PointValue : Points) {
					const TArray<TSharedPtr<FJsonValue>>& Pt = PointValue->AsArray();

					if (Pt.Num() >= 2) {
						double X = Pt[0]->AsNumber();
						double Y = Pt[1]->AsNumber();

						if (X < minx) minx = X;
						if (Y < miny) miny = Y;
					}
				}
			}
		}
	}

	for (const auto& FeatureValue : *FeaturesArray) {
		TSharedPtr<FJsonObject> FeatureObj = FeatureValue->AsObject();
		if (!FeatureObj.IsValid()) continue;

		FBuildingData bData;

		// 1. properties에서 층수 가져오기
		TSharedPtr<FJsonObject> PropertiesObj = FeatureObj->GetObjectField(TEXT("properties"));
		bData.FloorCount = PropertiesObj->HasField(TEXT("floor")) ? PropertiesObj->GetIntegerField(TEXT("floor")) : 1;

		// 2. geometry 및 coordinates 추출
		TSharedPtr<FJsonObject> GeometryObj = FeatureObj->GetObjectField(TEXT("geometry"));
		if (!GeometryObj.IsValid()) continue;

		const TArray<TSharedPtr<FJsonValue>>* Coordinates = nullptr;
		if (GeometryObj->TryGetArrayField(TEXT("coordinates"), Coordinates) && Coordinates->Num() > 0) {

			const TArray<TSharedPtr<FJsonValue>>& Ring = (*Coordinates)[0]->AsArray();

			if (Ring.Num() > 0) {
				const TArray<TSharedPtr<FJsonValue>>& Points = Ring[0]->AsArray();

				FVector SumLocation = FVector::ZeroVector;

				for (const auto& PointValue : Points) {
					const TArray<TSharedPtr<FJsonValue>>& Pt = PointValue->AsArray();

					if (Pt.Num() >= 2) {
						double RawX = Pt[0]->AsNumber();
						double RawY = Pt[1]->AsNumber();

						float LocalX = (RawX - minx) * BuildingBetweenDistance;
						float LocalY = -(RawY - miny) * BuildingBetweenDistance;

						FVector Vertex = FVector(LocalX, LocalY, 0.0f);
						bData.Vertices.Add(Vertex);
						SumLocation += Vertex;
					}
				}

				if (bData.Vertices.Num() > 0) {
					bData.CenterLocation = SumLocation / bData.Vertices.Num();
					BuildingData.Add(bData);
				}
			}
		}
	}

	BuildingData.Sort(sortBuildingData);

	FVector TargetCenter = FVector(0.f, 0.f, 0.f);

	//for (int i = 0; i < 10; i++) {
		for (int i = 0; i < BuildingData.Num(); i++) {
		const TArray<FVector>& Vertices = BuildingData[i].Vertices;
		if (Vertices.Num() == 0) continue;

		// 2. 이 건물의 실제 가로(X)와 세로(Y) 크기를 계산합니다.
		float MaxX = -1e9f;
		float MaxY = -1e9f;
		float MinX = 1e9f;
		float MinY = 1e9f;

		for (const FVector& Vertex : Vertices) {
			if (Vertex.X > MaxX) MaxX = Vertex.X;
			if (Vertex.Y > MaxY) MaxY = Vertex.Y;
			if (Vertex.X < MinX) MinX = Vertex.X;
			if (Vertex.Y < MinY) MinY = Vertex.Y;
		}

		float WidthX = MaxX - MinX;
		float LengthY = MaxY - MinY;

		int floor = BuildingData[i].FloorCount;

		FVector P1 = BuildingData[i].Vertices[0];
		FVector P2 = BuildingData[i].Vertices[1];
		FVector Direction = (P2 - P1).GetSafeNormal();

		// 3. 방향 벡터를 언리얼의 회전값(FRotator)으로 변환
		FRotator BuildingRotator = Direction.Rotation();

		// 건물은 땅에 평평하게 서 있어야 하므로 롤(Roll)과 피치(Pitch)는 0으로 묶고, 
		// 좌우 회전값인 요(Yaw)만 사용합니다.
		BuildingRotator.Pitch = 0.0f;
		BuildingRotator.Roll = 0.0f;

		FVector SpawnLocation = BuildingData[i].CenterLocation - FVector(WidthX * 0.5f, LengthY * 0.5f, 0.f);

		FRotator rotator = FRotator::ZeroRotator;

		AABuildingBase* Actor = world->SpawnActor<AABuildingBase>(BuildingBase, SpawnLocation, BuildingRotator);

		if (Actor)
		{
			Actor->SetBuildingTransform(WidthX, LengthY, floor);
		}
	}
	retFlag = false;
}

void UUCityNewworkManager::BuildNavigationNetwork()
{

	TSet<FVector> NodePositionSet;
	int NodeId = 0;
	for (int RoadIndex = 0; RoadIndex < OutRoadVector.Num(); RoadIndex++) {
		ARoadActor* RoadActor = OutRoadVector[RoadIndex];
		for (int i = 0; i < RoadActor->WorldPoints.Num(); i++) {
			int64 intX = FMath::RoundToInt(RoadActor->WorldPoints[i].X);
			int64 intY = FMath::RoundToInt(RoadActor->WorldPoints[i].Y);
			int64 intZ = FMath::RoundToInt(RoadActor->WorldPoints[i].Z);

			FVector FV(intX, intY, intZ);

			NodePositionSet.Add(FV);
		}
	}

	TMap<FVector, int32> NodeMap;
	Nodes.SetNum(NodePositionSet.Num());

	int32 NodeID = 0;
	for (const FVector&Position : NodePositionSet) {
		FRoadNode newNode;
		newNode.Location = Position;
		newNode.NodeID = NodeID;

		NodeMap.Add({ Position, NodeID });
		Nodes[NodeID] = newNode;

		NodeID++;
	}

	int32 EdgeID = 0;
	Edges.Empty(); // 에지 배열 초기화

	for (int RoadIndex = 0; RoadIndex < OutRoadVector.Num(); RoadIndex++) {
		ARoadActor* RoadActor = OutRoadVector[RoadIndex];
		if (!RoadActor) continue;

		for (int i = 0; i < RoadActor->WorldPoints.Num() - 1; i++) {
			FVector PosA = FVector(
				FMath::RoundToInt(RoadActor->WorldPoints[i].X),
				FMath::RoundToInt(RoadActor->WorldPoints[i].Y),
				FMath::RoundToInt(RoadActor->WorldPoints[i].Z));

			FVector PosB = FVector(
				FMath::RoundToInt(RoadActor->WorldPoints[i + 1].X),
				FMath::RoundToInt(RoadActor->WorldPoints[i + 1].Y),
				FMath::RoundToInt(RoadActor->WorldPoints[i + 1].Z));

			int32* NodeAPTR = NodeMap.Find(PosA);
			int32* NodeBPTR = NodeMap.Find(PosB);

			if (!NodeAPTR || !NodeBPTR) continue;
			if (*NodeAPTR == *NodeBPTR) continue; // 같은 위치면 패스

			int32 NodeAID = *NodeAPTR;
			int32 NodeBID = *NodeBPTR;

			// 무방향 에지 생성
			FRoadEdge NewEdge;
			NewEdge.EdgeID = EdgeID;
			NewEdge.NodeA = NodeAID;
			NewEdge.NodeB = NodeBID;
			NewEdge.Distance = FVector::Distance(PosA, PosB);
			NewEdge.OwnerRoadActor = RoadActor;
			NewEdge.SegmentIndex = i; // 필요 시 맵 매핑용

			Edges.Add(NewEdge);

			// [핵심] 두 노드 모두에게 이 에지 ID를 등록 (무방향 연결)
			Nodes[NodeAID].ConnectingEdgeIDs.Add(EdgeID);
			Nodes[NodeBID].ConnectingEdgeIDs.Add(EdgeID);

			EdgeID++;
		}
	}
	//UE_LOG(LogTemp, Log, TEXT("NodeCount : %d, EdgeCount: %d"), Nodes.Num(), Edges.Num());
}

TArray<FRoadNode> UUCityNewworkManager::Navigation(AProjectCharacter* player, const FVector PlayerLocation)
{
	SelectNode; //이거 초기화 해야함
	maxNodeCount = 1e9;
	double mindist = 1e9;
	NavigationCourse.Empty();
	//이분 탐색으로 변경해야함
	for (FRoadNode &Node : Nodes) {
		if (Node.Location.X == 0.f && Node.Location.Y == 0.f && Node.Location.Z == 0.f) {
			continue;
		}
		FVector NodeLocation = Node.Location;

		double dist = FVector::Dist(PlayerLocation, NodeLocation);

		if (dist < mindist) {
			mindist = dist;
			SelectNode = Node;
		}
	}

	if (!player) return NavigationCourse;

	TArray<bool> visit;
	visit.Init(false, Nodes.Num()); // 지난번 이야기한 올바른 초기화법

	
	int GoalNodeID = 50;

	FVector GoalLocation = Nodes[GoalNodeID].Location;

	NavigationCourse = DfsNavigation(SelectNode.NodeID, 1, GoalNodeID);

	if (NavigationCourse.Num() == 0) {
		UE_LOG(LogTemp, Error, TEXT("FinalCourse Size = 0"));
	}
	return NavigationCourse;
}

struct Data {
	TArray<FRoadNode> FinalCourse;
	int Node;
	double distance;
};

struct FDataPredicate
{
	FORCEINLINE bool operator()(const Data& A, const Data& B) const
	{
		return A.distance < B.distance; // 최소 힙 (거리가 짧은 게 위로)
	}
};

TArray<FRoadNode> UUCityNewworkManager::DfsNavigation(int CurrentNodeID, int NodeCount, int GoalNodeID)
{
	TArray<FRoadNode> FinalCourse;

	TArray<Data> NodeHeap;
	TArray<bool> visit;
	visit.Init(false, Nodes.Num());

	// 시작 노드 방문 처리 및 초기 데이터 설정
	visit[CurrentNodeID] = true;

	Data StartData;
	StartData.Node = CurrentNodeID;
	StartData.FinalCourse.Add(Nodes[CurrentNodeID]); // 시작 노드 추가

	NodeHeap.HeapPush(StartData, FDataPredicate());

	int IntMax = 1e9;
	TArray<int32> MinDistance;
	MinDistance.Init(IntMax, Nodes.Num()); // IntMax는 매우 큰 값

	// 시작 노드의 거리는 0으로 설정
	MinDistance[CurrentNodeID] = 0;

	while (NodeHeap.Num() > 0) {
		Data qd;
		NodeHeap.HeapPop(qd, FDataPredicate());

		//Distance랑 비교해서 짧으면 return
		if (qd.Node == GoalNodeID) {
			FinalCourse = qd.FinalCourse; // 배열 내용물 복사
			break;
		}

		for (int i = 0; i < Nodes[qd.Node].ConnectingEdgeIDs.Num(); i++) {
			int32 EdgeID = Nodes[qd.Node].ConnectingEdgeIDs[i];
			FRoadEdge Edge = Edges[EdgeID];

			// 무방향 판별: 현재 노드가 NodeA면 다음은 NodeB, 반대면 NodeA
			int32 NextNodeID = (Edge.NodeA == qd.Node) ? Edge.NodeB : Edge.NodeA;

			// 이미 더 짧은 경로로 방문했다면 패스
			int32 NewDistance = qd.distance + Edge.Distance;
			if (NewDistance >= MinDistance[NextNodeID]) continue;

			// 최단 거리 기록 갱신
			MinDistance[NextNodeID] = NewDistance;

			// 다음 탐색 데이터 생성
			Data NextData;
			NextData.Node = NextNodeID;
			NextData.FinalCourse = qd.FinalCourse;
			NextData.FinalCourse.Add(Nodes[NextNodeID]); // 경로에 추가
			NextData.distance = NewDistance;

			// 우선순위 큐(또는 힙)에 푸시
			NodeHeap.HeapPush(NextData, FDataPredicate());
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Final Course Num : %d"), FinalCourse.Num());

	// 안전하게 경로 검사 (최소 2개 이상의 노드가 있어야 선을 이을 수 있음)
	if (FinalCourse.Num() >= 2)
	{
		for (int32 CourseIdx = 0; CourseIdx < FinalCourse.Num() - 1; CourseIdx++)
		{
			FRoadNode& CurrentNode = (FinalCourse)[CourseIdx];
			FRoadNode& NextNode = (FinalCourse)[CourseIdx + 1];

			ARoadActor* FoundRoadActor = nullptr;
			int32 FoundSegmentIndex = -1; // [해결책] 세그먼트 인덱스를 저장할 외부 변수 선언

			// 현재 노드에 연결된 에지들을 순회
			for (int32 EdgeID : CurrentNode.ConnectingEdgeIDs)
			{
				FRoadEdge& Edge = Edges[EdgeID];

				// 두 노드를 연결하는 무방향 에지인지 검사
				if ((Edge.NodeA == CurrentNode.NodeID && Edge.NodeB == NextNode.NodeID) ||
					(Edge.NodeB == CurrentNode.NodeID && Edge.NodeA == NextNode.NodeID))
				{
					if (Edge.OwnerRoadActor)
					{
						FoundRoadActor = Edge.OwnerRoadActor;
						FoundSegmentIndex = Edge.SegmentIndex; // [해결책] 스택에서 사라지기 전에 값을 복사!
					}
					break;
				}
			} // 여기서 Edge 변수는 스택에서 사라집니다.

			// [안전 구역] 외부 변수에 복사해 뒀으므로 에러 없이 안전하게 호출 가능!
			if (FoundRoadActor && FoundSegmentIndex != -1)
			{
				FoundRoadActor->ChangeRoadColor(FoundSegmentIndex, FColor::Green);
			}
		}
	}

	return FinalCourse;
}




