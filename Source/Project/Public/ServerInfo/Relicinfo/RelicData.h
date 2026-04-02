// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include "RelicData.generated.h"

USTRUCT(BlueprintType)
struct FRelicData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FName RelicId;

	UPROPERTY()
	FName RelicImageId;

	UPROPERTY()
	int Level;

	UPROPERTY()
	float HP;

	UPROPERTY()
	float Attack;
	
	UPROPERTY()
	float Defence;
	
	UPROPERTY()
	float Force;
	
	UPROPERTY()
	float Critical;
	
	UPROPERTY()
	float CriticalDamage;

};
