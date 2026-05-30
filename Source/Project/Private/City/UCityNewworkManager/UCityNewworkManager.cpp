// Fill out your copyright notice in the Description page of Project Settings.


#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "City/Building/BuildingBase/ABuildingBase.h"
#include "City/DataAsset/BuildingDataAsset.h"
#include <algorithm>

using namespace std;

struct FBuildingData {
	int32 FloorCount;
	FVector CenterLocation;
	TArray<FVector> Vertices; // 정렬된 최종 로컬 좌표들
};

bool sortBuildingData(const FBuildingData& A, const FBuildingData& B) {
	double DistA = (A.CenterLocation.X * A.CenterLocation.X) + (A.CenterLocation.Y * A.CenterLocation.Y);

	// B 건물의 원점(0,0)과의 거리 제곱 (X^2 + Y^2)
	double DistB = (B.CenterLocation.X * B.CenterLocation.X) + (B.CenterLocation.Y * B.CenterLocation.Y);

	// 거리가 더 짧은(가까운) 놈이 앞으로 오도록 정렬
	return DistA < DistB;
}

void UUCityNewworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	bool retFlag;
	LoadBuildingDataAsset(retFlag);
	if (retFlag) return;
	
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
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NO ASSETCLASS"));
	}
	retFlag = false;
}

void UUCityNewworkManager::LoadQGIS()
{
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
						float LocalY = (RawY - miny) * BuildingBetweenDistance;

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

	for (int i = 0; i < 1000; i++) {
	//for (int i = 0; i < BuildingData.Num(); i++) {
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

		// 꼭짓점 간의 차이가 이 건물의 실제 크기 (단위: cm)
		float WidthX = MaxX - MinX;
		float LengthY = MaxY - MinY;

		// 3. 층수를 반영한 높이 계산 (층당 300cm = 3m 가정)
		int floor = BuildingData[i].FloorCount;

		// 4. 건물의 중심점을 기준으로 오프셋을 빼서 스폰 위치 계산
		// (피벗이 좌측 하단 모서리에 있으므로 크기의 절반만큼 빼줌)
		FVector SpawnLocation = BuildingData[i].CenterLocation - FVector(WidthX * 0.5f, LengthY * 0.5f, 0.f);

		FRotator rotator = FRotator::ZeroRotator;

		AABuildingBase* Actor = world->SpawnActor<AABuildingBase>(BuildingBase, SpawnLocation, rotator);

		if (Actor)
		{
			Actor->SetBuildingTransform(WidthX, LengthY, floor);
		}
	}

}
