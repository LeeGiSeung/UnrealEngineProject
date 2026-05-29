// Fill out your copyright notice in the Description page of Project Settings.


#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "City/Building/BuildingBase/ABuildingBase.h"
#include "City/DataAsset/BuildingDataAsset.h"

void UUCityNewworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	//CityManager에서 QGIS로 긁어온 Json에 접근해서 
	//각각 층수, 위치에 맞게 소환함

	//일단 테스트 위해서 0,0 좌표로

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

	FVector TargetCenter = FVector(-1400.f, -500.f, 0.f);

	float WidthX = 100.f;
	float LengthY = 300.f;
	int floor = 10;

	FVector SpawnLocation = TargetCenter - FVector(WidthX * 0.5f, LengthY * 0.5f, 0.f);

	FRotator fr = FRotator::ZeroRotator;

	AABuildingBase* Actor = world->SpawnActor<AABuildingBase>(BuildingBase, SpawnLocation, fr);

	if (Actor)
	{
		Actor->SetBuildingTransform(WidthX, LengthY, 10);
	}
}
