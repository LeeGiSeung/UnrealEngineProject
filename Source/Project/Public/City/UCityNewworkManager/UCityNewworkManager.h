// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UCityNewworkManager.generated.h"

class AABuildingBase;

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

private:

	void LoadQGIS();

	float standX = -1400.f;
	float standY = -500.f;
};
