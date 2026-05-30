// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BuildingDataAsset.generated.h"

class AABuildingBase;

/**
 * 
 */
UCLASS()
class PROJECT_API UBuildingDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "BuildingBase")
	TSubclassOf<AABuildingBase> BuildingBase;

	UPROPERTY(EditAnywhere, Category = "BuildingBase")
	float BuildingBetweenDistance = 100.f;
};
