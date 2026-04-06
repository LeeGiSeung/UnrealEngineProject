// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FMaininfo.generated.h"

USTRUCT(BlueprintType)
struct FMaininfo : public FTableRowBase {

	GENERATED_BODY();

	UPROPERTY()
	int Level;

	UPROPERTY()
	float LevelEXP;

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