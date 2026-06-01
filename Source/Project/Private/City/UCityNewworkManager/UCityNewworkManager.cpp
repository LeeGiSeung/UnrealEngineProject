// Fill out your copyright notice in the Description page of Project Settings.


#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "City/Building/BuildingBase/ABuildingBase.h"
#include "City/DataAsset/BuildingDataAsset.h"
#include "City/RoadActor/RoadActor.h"
#include <algorithm>
#include "Kismet/GameplayStatics.h"

using namespace std;



struct FBuildingData {
	int32 FloorCount;
	FVector CenterLocation;
	TArray<FVector> Vertices; // 정렬된 최종 로컬 좌표들
};

struct FRoadData {
	FString RoadName;       // 명칭
	double RoadWidth;       // 도로폭
	int32 LaneCount;        // 차로수
	TArray<FVector> Points; // 도로를 구성하는 정점들 (Line)
};

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
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NO ASSETCLASS"));
	}
	retFlag = false;
}

void UUCityNewworkManager::LoadQGIS()
{
	//################################################
	//빌딩 데이터 Load
	//################################################
	bool BuildingFlag;
	LoadBuilding(BuildingFlag);
	if (BuildingFlag) return;

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
			PropertiesObj->TryGetStringField(TEXT("RoadName"), rData.RoadName);
			PropertiesObj->TryGetNumberField(TEXT("RoadWidth"), rData.RoadWidth);
			PropertiesObj->TryGetNumberField(TEXT("RoadCount"), rData.LaneCount);
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

	for (int i = 0; i < RoadDataList.Num(); i++) {
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

				roadactor->SpawnRoadActor(LocalPoints, RoadData.RoadWidth);

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

	int32 NodeID = 0;
	for (const FVector&Position : NodePositionSet) {
		FRoadNode newNode;
		newNode.Location = Position;
		newNode.NodeID = NodeID;

		NodeMap.Add({ Position, NodeID });
		NodeID++;
	}

	int EdgeID = 0;

	Nodes.SetNum(NodePositionSet.Num());

	for (int RoadIndex = 0; RoadIndex < OutRoadVector.Num(); RoadIndex++) {
		ARoadActor* RoadActor = OutRoadVector[RoadIndex];
		if (!RoadActor || RoadActor->WorldPoints.Num() < 2) continue;

		for (int i = 0; i < RoadActor->WorldPoints.Num() - 1; i++) {

			FRoadEdge NewEdge;

			FVector StartPos = 
				FVector(
					FMath::RoundToInt(RoadActor->WorldPoints[i].X), 
					FMath::RoundToInt(RoadActor->WorldPoints[i].Y), 
					FMath::RoundToInt(RoadActor->WorldPoints[i].Z));

			FVector EndPos = 
				FVector(
					FMath::RoundToInt(RoadActor->WorldPoints[i + 1].X), 
					FMath::RoundToInt(RoadActor->WorldPoints[i + 1].Y), 
					FMath::RoundToInt(RoadActor->WorldPoints[i + 1].Z));

			int32* StartNodePTR = NodeMap.Find(StartPos);
			int32* EndNodePTR = NodeMap.Find(EndPos);

			if (!StartNodePTR || !EndNodePTR) continue;
			if (StartNodePTR == EndNodePTR) continue;

			int32 StartNodeID = *StartNodePTR;
			int32 EndNodeID = *EndNodePTR;

			NewEdge.StartNodeID = NodeMap[StartPos];
			NewEdge.EndNodeID = NodeMap[EndPos];
			NewEdge.Distance = FVector::Distance(StartPos, EndPos);
			NewEdge.EdgeID = EdgeID;
			NewEdge.OwnerRoadActor = RoadActor;
			NewEdge.SegmentIndex = i;

			Edges.Add(NewEdge);

			Nodes[StartNodeID].ConnectingEdgeID.Add(EdgeID);
			Nodes[EndNodeID].ConnectingEdgeID.Add(EdgeID);

			EdgeID++;
		}

	}
	UE_LOG(LogTemp, Log, TEXT("NodeCount : %d, EdgeCount: %d"), Nodes.Num(), Edges.Num());
}

