// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FSkillInfo.generated.h"

USTRUCT(BlueprintType)
struct FSkillInfo : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY()
	int AutoAttackLevel;

	UPROPERTY()
	bool AutoAttackNode_0;

	UPROPERTY()
	bool AutoAttackNode_1;

	UPROPERTY()
	bool AutoAttackNode_2;

	UPROPERTY()
	int EAttackLevel;

	UPROPERTY()
	bool EAttackkNode_0;

	UPROPERTY()
	bool EAttackNode_1;

	UPROPERTY()
	bool EAttackNode_2;

	UPROPERTY()
	int RAttackLevel;

	UPROPERTY()
	bool RAttackNode_0;

	UPROPERTY()
	bool RAttackNode_1;

	UPROPERTY()
	bool RAttackNode_2;

	UPROPERTY()
	int PAttackLevel;

	UPROPERTY()
	bool PAttackNode_0;

	UPROPERTY()
	bool PAttackNode_1;

	UPROPERTY()
	bool PAttackNode_2;

};