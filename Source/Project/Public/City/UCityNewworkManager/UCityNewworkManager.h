// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UCityNewworkManager.generated.h"

class AABuildingBase;
class ARoadActor;


/**
 * 
 */
UCLASS()
class PROJECT_API UUCityNewworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadBuildingDataAsset(bool& retFlag);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AABuildingBase> BuildingBase;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ARoadActor> RoadActorClass;

	UPROPERTY()
	float BuildingBetweenDistance;

private:

	void LoadQGIS();

	void LoadRoad(bool& retFlag);

	void RoadBuilding(bool& retFlag);

	float standX = -1400.f;
	float standY = -500.f;
};
