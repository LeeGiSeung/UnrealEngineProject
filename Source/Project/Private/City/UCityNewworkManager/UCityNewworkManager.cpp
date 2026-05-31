// Fill out your copyright notice in the Description page of Project Settings.


#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "City/Building/BuildingBase/ABuildingBase.h"
#include "City/DataAsset/BuildingDataAsset.h"
#include "City/RoadActor/RoadActor.h"
#include <algorithm>

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
	//도로 데이터 Load
	//################################################
	bool BuildingFlag;
	RoadBuilding(BuildingFlag);
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

	// === 1. 최소 X, Y 좌표 구하기 (안전성 조건 추가) ===
	for (const auto& FeatureValue : *FeaturesArray) {
		TSharedPtr<FJsonObject> FeatureObj = FeatureValue->AsObject();
		if (!FeatureObj.IsValid()) continue;

		TSharedPtr<FJsonObject> GeometryObj = FeatureObj->GetObjectField(TEXT("geometry"));
		if (!GeometryObj.IsValid()) continue;

		// [수정] 첫 번째 루프에서도 반드시 MultiLineString 데이터만 골라내야 크래시가 안 납니다!
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

	// [수정] .Num()을 출력할 때는 %d 대신 %e나 안전하게 자료형을 맞추거나 언리얼 표준 포맷인 %d 툴 대신 다음과 같이 씁니다.
	UE_LOG(LogTemp, Log, TEXT("Total Roads Parsed: %d"), RoadDataList.Num());

	// 정렬 함수 (구현해 두신 sortroad 함수 기준)
	RoadDataList.Sort(sortroad);

	// === 3. 스폰 (상대 좌표 변환 적용 버전) ===
	for (int i = 0; i < RoadDataList.Num(); i++) {
		const FRoadData& RoadData = RoadDataList[i];

		// 1. 스폰 위치 결정 (이 위치가 기즈모의 위치가 됩니다)
		FVector RoadSpawnLocation = RoadData.Points[0];
		FRotator Roadrotator = FRotator::ZeroRotator;

		if (RoadActorClass) {
			ARoadActor* roadactor = world->SpawnActor<ARoadActor>(RoadActorClass, RoadSpawnLocation, Roadrotator);
			if (roadactor) {
				// 2. [핵심 수정] 모든 점을 '액터의 위치(Points[0])' 기준으로 상대 좌표화합니다.
				TArray<FVector> LocalPoints;
				for (const FVector& GlobalPt : RoadData.Points) {
					// 첫 번째 점을 빼줌으로써 첫 번째 점은 (0,0,0)이 되고 나머지는 상대 거리가 됩니다.
					LocalPoints.Add(GlobalPt - RoadSpawnLocation);
				}

				// 이제 로컬 좌표가 된 배열을 전달합니다.
				roadactor->SpawnRoadActor(LocalPoints, RoadData.RoadWidth);
			}
		}
	}

	retFlag = false;
}

void UUCityNewworkManager::RoadBuilding(bool& retFlag)
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

