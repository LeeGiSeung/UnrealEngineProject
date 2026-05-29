// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/Building/BuildingBase/ABuildingBase.h"


#include "ABuildingHouse.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AABuildingHouse : public AABuildingBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float tick) override;

};
