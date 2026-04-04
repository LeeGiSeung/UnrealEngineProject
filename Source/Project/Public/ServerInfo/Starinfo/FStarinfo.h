// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include "FStarinfo.generated.h"

USTRUCT(BlueprintType)
struct FStarinfo : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY()
	FName CharacterID;

	UPROPERTY()
	int StarLevel;
};
