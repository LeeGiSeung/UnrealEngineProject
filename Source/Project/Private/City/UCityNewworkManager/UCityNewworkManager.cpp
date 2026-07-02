// Fill out your copyright notice in the Description page of Project Settings.


#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "City/Building/BuildingBase/ABuildingBase.h"
#include "City/DataAsset/BuildingDataAsset.h"
#include "City/RoadActor/RoadActor.h"
#include <algorithm>
#include "Kismet/GameplayStatics.h"
#include "ProjectCharacter.h"
#include "Containers/Queue.h"
#include <float.h>
#include "City/MapWidget/Marker/MapViewer/PointMarker.h"

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

	PurposeMap.Add(TEXT("OtherFacilities"), EBuildingType::OtherFacilities);
	PurposeMap.Add(TEXT("FacilitiesRelatedtoAnimalsandPlants"), EBuildingType::FacilitiesRelatedtoAnimalsandPlants);
	PurposeMap.Add(TEXT("AutomotiveRelatedFacilities"), EBuildingType::AutomotiveRelatedFacilities);
	PurposeMap.Add(TEXT("Accommodation"), EBuildingType::Accommodation);
	PurposeMap.Add(TEXT("ReligiousFacilities"), EBuildingType::ReligiousFacilities);
	PurposeMap.Add(TEXT("EducationalandResearchFacilities"), EBuildingType::EducationalandResearchFacilities);
	PurposeMap.Add(TEXT("HazardousMaterialsStorageandHandlingFacility"), EBuildingType::HazardousMaterialsStorageandHandlingFacility);
	PurposeMap.Add(TEXT("Facilitiesfortheelderlyandchildren"), EBuildingType::Facilitiesfortheelderlyandchildren);
	PurposeMap.Add(TEXT("Transportationfacilities"), EBuildingType::Transportationfacilities);
	PurposeMap.Add(TEXT("SportsFacilities"), EBuildingType::SportsFacilities);
	PurposeMap.Add(TEXT("WarehouseFacilities"), EBuildingType::WarehouseFacilities);
	PurposeMap.Add(TEXT("RetailFacilities"), EBuildingType::RetailFacilities);
	PurposeMap.Add(TEXT("OfficeFacilities"), EBuildingType::OfficeFacilities);
	PurposeMap.Add(TEXT("NightSoilandWasteTreatmentFacilities"), EBuildingType::NightSoilandWasteTreatmentFacilities);
	PurposeMap.Add(TEXT("Factory"), EBuildingType::Factory);
	PurposeMap.Add(TEXT("House"), EBuildingType::House);
	PurposeMap.Add(TEXT("MedicalFacilities"), EBuildingType::MedicalFacilities);
	PurposeMap.Add(TEXT("TrainingFacility"), EBuildingType::TrainingFacility);
	PurposeMap.Add(TEXT("CulturalandAssemblyFacilities"), EBuildingType::CulturalandAssemblyFacilities);
	PurposeMap.Add(TEXT("LivingFacility"), EBuildingType::LivingFacility);
	PurposeMap.Add(TEXT("Funeralfacility"), EBuildingType::Funeralfacility);
	PurposeMap.Add(TEXT("BroadcastingandCommunicationsFacilities"), EBuildingType::BroadcastingandCommunicationsFacilities);
	

	bool buildingFlag;
	LoadBuildingDataAsset(buildingFlag);
	if (buildingFlag) return;

	LoadQGIS();

	GetWorld()->GetTimerManager().SetTimer(VisibilityTimerHandle, this, &UUCityNewworkManager::CheckCityVisibility, 5.f, false);



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
		RoadBetweenDistance = LoadedConfig->RoadBetweenDistance;
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

void UUCityNewworkManager::GetMapBounds(float& OutMinX, float& OutMaxX, float& OutMinY, float& OutMaxY)
{
	OutMinX = WorldMinX;
	OutMaxX = WorldMaxX;
	OutMinY = WorldMinY;
	OutMaxY = WorldMaxY;
}

void UUCityNewworkManager::SetPointMarkerArray(TArray<UPointMarker*> _PointMarkerArray)
{
	PointMarkerArray = _PointMarkerArray;
}

void UUCityNewworkManager::LoadQGIS()
{

	LoadBuilding();
	LoadRoad();

	OnLocalXYSetting.Broadcast(WorldMinX, WorldMaxX, WorldMinY, WorldMaxY);

}

void UUCityNewworkManager::LoadRoad()
{
	UWorld* world = GetWorld();
	if (!world) {
		UE_LOG(LogTemp, Error, TEXT("NO WORLD OR BASE"));
		return;
	}
	RoadDataList.Empty();
	FString RelativePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("IncheonLandFile/IncheonData/Inchecon_Michuholgu_Road_All.geojson"));
	FString DataPath = FPaths::ConvertRelativePathToFull(RelativePath);

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *DataPath)) {
		UE_LOG(LogTemp, Warning, TEXT("No Road FILE~"));
		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Yes Road FILE~"));
	}

	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	TSharedPtr<FJsonObject> JsonObject;

	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid()) {
		UE_LOG(LogTemp, Error, TEXT("Fail string to json"));
		return;
	}

	const TArray<TSharedPtr<FJsonValue>>* FeaturesArray;
	if (!JsonObject->TryGetArrayField(TEXT("features"), FeaturesArray)) return;

	double minx = DBL_MAX;
	double miny = DBL_MAX;

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

					float LocalX = (RawX - minx) * RoadBetweenDistance;
					float LocalY = -((RawY - miny) * RoadBetweenDistance);

					WorldMinX = FMath::Min(WorldMinX, LocalX);
					WorldMaxX = FMath::Max(WorldMaxX, LocalX);
					WorldMinY = FMath::Min(WorldMinY, LocalY);
					WorldMaxY = FMath::Max(WorldMaxY, LocalY);

					rData.Points.Add(FVector(LocalX, LocalY, 0.0f));
				}
			}

			if (rData.Points.Num() > 1) {
				RoadDataList.Add(rData);
			}
		}
	}

	RoadDataList.Sort(sortroad);

	for (FRoadData& Data : RoadDataList) {
		FRuntimeRoadData RoadData;

		RoadData.Points = Data.Points;
		RoadData.RoadCount = Data.RoadCount;
		RoadData.RoadWidth = Data.RoadWidth;

		TotalRoadData.Add(RoadData);
	}
}

void UUCityNewworkManager::LoadBuilding()
{
	UWorld* world = GetWorld();
	if (!BuildingBase || !world) {
		UE_LOG(LogTemp, Error, TEXT("NO BUILDING BASE"));
		return;
	}

	FString RelativePath = FPaths::Combine(FPaths::ProjectDir(), TEXT("IncheonLandFile/IncheonData/Incheon_Michugolgu_All.geojson"));

	FString DataPath = FPaths::ConvertRelativePathToFull(RelativePath);

	FString JsonString;
	if (!FFileHelper::LoadFileToString(JsonString, *DataPath)) {
		UE_LOG(LogTemp, Error, TEXT("No Building FILE"));
		return;
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Yes Building FILE~"));
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

	double minx = DBL_MAX;
	double miny = DBL_MAX;

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

		FString Purpose;

		if (PropertiesObj->TryGetStringField(TEXT("purpose"), Purpose))
		{

			if (const EBuildingType* Type = PurposeMap.Find(Purpose))
			{
				bData.BuildingType = *Type;
			}
			else
			{
				bData.BuildingType = EBuildingType::None;

				UE_LOG(LogTemp, Warning, TEXT("%s is NO PurposeMap."), *Purpose);
			}
		}
		else
		{
			bData.BuildingType = EBuildingType::None;
		}


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

						WorldMinX = FMath::Min(WorldMinX, LocalX);
						WorldMaxX = FMath::Max(WorldMaxX, LocalX);
						WorldMinY = FMath::Min(WorldMinY, LocalY);
						WorldMaxY = FMath::Max(WorldMaxY, LocalY);

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

		/*const TArray<TSharedPtr<FJsonValue>>* Coordinates = nullptr;
		if (GeometryObj->TryGetArrayField(TEXT("coordinates"), Coordinates) && Coordinates->Num() > 0)
		{
			const TArray<TSharedPtr<FJsonValue>>& Ring = (*Coordinates)[0]->AsArray();

			if (Ring.Num() > 0)
			{
				const TArray<TSharedPtr<FJsonValue>>& Points = Ring[0]->AsArray();

				float MinX = FLT_MAX;
				float MinY = FLT_MAX;
				float MaxX = -FLT_MAX;
				float MaxY = -FLT_MAX;

				for (const auto& PointValue : Points)
				{
					const TArray<TSharedPtr<FJsonValue>>& Pt = PointValue->AsArray();

					if (Pt.Num() >= 2)
					{
						double RawX = Pt[0]->AsNumber();
						double RawY = Pt[1]->AsNumber();

						float LocalX = (RawX - minx) * BuildingBetweenDistance;
						float LocalY = -(RawY - miny) * BuildingBetweenDistance;

						WorldMinX = FMath::Min(WorldMinX, LocalX);
						WorldMaxX = FMath::Max(WorldMaxX, LocalX);
						WorldMinY = FMath::Min(WorldMinY, LocalY);
						WorldMaxY = FMath::Max(WorldMaxY, LocalY);

						MinX = FMath::Min(MinX, LocalX);
						MaxX = FMath::Max(MaxX, LocalX);
						MinY = FMath::Min(MinY, LocalY);
						MaxY = FMath::Max(MaxY, LocalY);

						bData.Vertices.Add(FVector(LocalX, LocalY, 0.f));
					}
				}

				if (bData.Vertices.Num() > 0)
				{
					bData.CenterLocation = FVector(
						(MinX + MaxX) * 0.5f,
						(MinY + MaxY) * 0.5f,
						0.f);

					BuildingData.Add(bData);
				}
			}
		}*/

	}

	BuildingData.Sort(sortBuildingData);

	FVector TargetCenter = FVector(0.f, 0.f, 0.f);

	for (int i = 0; i < BuildingData.Num(); i++) {
		const TArray<FVector>& Vertices = BuildingData[i].Vertices;
		if (Vertices.Num() == 0) continue;

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

		BuildingRotator.Pitch = 0.0f;
		BuildingRotator.Roll = 0.0f;
		BuildingRotator.Yaw += -90.f;

		FVector SpawnLocation = BuildingData[i].CenterLocation - FVector(WidthX * 0.5f, LengthY * 0.5f, 0.f);

		TSharedPtr<FRuntimeBuildingData> BuildData = MakeShared<FRuntimeBuildingData>();

		BuildData->SpawnLocation = SpawnLocation;
		BuildData->FloorCount = floor;
		BuildData->WidthX = WidthX;
		BuildData->LengthY = LengthY;
		BuildData->Rotation = BuildingRotator;
		BuildData->SpawnedActor = nullptr;
		BuildData->BuildingType = BuildingData[i].BuildingType;

		TotalBuildingData.Add(BuildData);
	}


}



void UUCityNewworkManager::UpdateBuildingVisibility(FVector PlayerLocation)
{

	UWorld* world = GetWorld();

	TArray<float> TestCheck;
	TArray<float> asdfCheck;
	int spawncount = 0;

	if (!world || TotalBuildingData.Num() == 0) return;

	int32 DataCount = 0; // long 대신 int32 추천

	for (const TSharedPtr<FRuntimeBuildingData>& DataPtr : TotalBuildingData)
	{
		// 1. 스마트 포인터 자체가 유효한지 널 체크
		if (!DataPtr.IsValid()) { DataCount++; continue; }

		// 2. 데이터 접근을 전부 '.'에서 '->'로 변경
		if (DataPtr->FloorCount == 0) { DataCount++; continue; }
		if (DataPtr->bOverlapRoad) { DataCount++; continue; }

		FVector SpawnLocation = DataPtr->SpawnLocation;
		FRotator BuildingRotator = DataPtr->Rotation;

		double distance = FVector::Distance(PlayerLocation, SpawnLocation);

		// [Case A] 플레이어가 스폰 거리 안으로 들어옴
		if (distance < MinComputeDistance)
		{
			// 3. 이미 스폰된 액터가 있다면 중복 스폰하지 않고 패스
			if (DataPtr->SpawnedActor != nullptr)
			{
				DataCount++;
				continue;
			}

			AABuildingBase* Actor = world->SpawnActor<AABuildingBase>(BuildingBase, SpawnLocation, BuildingRotator);
			
			if (Actor)
			{
				Actor->SetBuildingTransform(DataPtr->WidthX, DataPtr->LengthY, DataPtr->FloorCount, DataPtr->BuildingType);

				// 4. 액터에게 이 데이터의 TSharedPtr 주소록을 그대로 주입
				Actor->MyRuntimeData = DataPtr;

				// 5. 원본 데이터에도 스폰된 액터 주소 기록
				DataPtr->SpawnedActor = Actor;
			}
		}
		// [Case B] 플레이어가 스폰 거리보다 멀어짐 (디스폰 영역)
		else
		{
			TestCheck.Add(distance);

			if (DataPtr->SpawnedActor != nullptr)
			{
				// 액터를 안전하게 파괴하고 포인터 정리
				DataPtr->SpawnedActor->Destroy();
				DataPtr->SpawnedActor = nullptr;
			}
		}

		DataCount++;
	}
}

void UUCityNewworkManager::UpdateRoadVisibility(FVector PlayerLocation)
{
	UWorld* World = GetWorld();
	if (!World || !RoadActorClass || TotalRoadData.Num() == 0) return;

	OutRoadVector.Empty();
	int spawncount = 0;
	for (FRuntimeRoadData& RoadData : TotalRoadData) {

		FVector RoadSpawnLocation = RoadData.Points[0];
		FRotator Roadrotator = FRotator::ZeroRotator;

		double distance = FVector::Distance(RoadSpawnLocation, PlayerLocation);

		if (distance < MinComputeDistance) {

			if (RoadActorClass) {
				ARoadActor* roadactor = World->SpawnActor<ARoadActor>(RoadActorClass, RoadSpawnLocation, Roadrotator);
				if (roadactor) {

					TArray<FVector> LocalPoints;
					for (const FVector& GlobalPt : RoadData.Points) {
						LocalPoints.Add(GlobalPt - RoadSpawnLocation);
					}

					roadactor->SpawnRoadActor(LocalPoints, RoadData.RoadCount, RoadData.RoadWidth);

					roadactor->SetWorldPoints(RoadData.Points);

					RoadData.SpawnedActor = roadactor;
				}

				OutRoadVector.Add(roadactor);
			}
		}
		else {
			if (RoadData.SpawnedActor) {
				RoadData.SpawnedActor->Destroy();
				RoadData.SpawnedActor = nullptr;
			}
		}
	}

	BuildNavigationNetwork();

}

void UUCityNewworkManager::CheckCityVisibility()
{
	APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) return;

	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	// 확인된 두 배열의 데이터를 기반으로 거리 체크 및 스폰/디스폰 실행
	UpdateBuildingVisibility(PlayerLocation);
	UpdateRoadVisibility(PlayerLocation);
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
	double minplayerdist = 1e9;
	double mingoaldist = 1e9;
	NavigationCourse.Empty();
	

	if (!player) return NavigationCourse;

	int GoalNodeID = 0;
	FVector GoalLocation = PointMarkerArray[PointMarkerArray.Num() - 1]->GetMarkerPosition();

	//FVector GoalLocation(443852.649387, -426473.010608, 92.400002);

	UE_LOG(LogTemp, Error, TEXT("CityNetWorkManager GoalLocation : %s"), *GoalLocation.ToString());

	//이분 탐색으로 변경해야함
	for (FRoadNode &Node : Nodes) {
		if (Node.Location.X == 0.f && Node.Location.Y == 0.f && Node.Location.Z == 0.f) {
			continue;
		}
		FVector NodeLocation = Node.Location;

		double Playerdist = FVector::Dist(PlayerLocation, NodeLocation);
		
		double Goaldist = FVector::Dist(GoalLocation, NodeLocation);

		if (Playerdist < minplayerdist) {
			minplayerdist = Playerdist;
			SelectNode = Node;
		}

		if (Goaldist < mingoaldist) {
			mingoaldist = Goaldist;
			GoalNode = Node;
			GoalNodeID = Node.NodeID;
		}
	}

	TArray<bool> visit;
	visit.Init(false, Nodes.Num()); // 지난번 이야기한 올바른 초기화법

	//int GoalNodeID = 50;
	
	//FVector GoalLocation = Nodes[GoalNodeID].Location;

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
						FoundSegmentIndex = Edge.SegmentIndex; // [해결책] 스택에서 사라지기 전에 값 복사
					}
					break;
				}
			} 

			if (FoundRoadActor && FoundSegmentIndex != -1)
			{
				FoundRoadActor->ChangeRoadColor(FoundSegmentIndex, FColor::Green);
			}
		}
	}

	return FinalCourse;
}




