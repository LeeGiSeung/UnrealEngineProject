// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelSequence.h"

#include "EventRow.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FEventRow : public FTableRowBase {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ULevelSequence> LevelSequence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LoopFrame;
};
