// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueType/DialogueType.h"
#include "AutoDialogueRow.generated.h"

USTRUCT(BlueprintType)
struct FAutoDialogueRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName Speaker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText FirstText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SecondText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDialogueUIType UIType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName NextID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName DirectingKey;
};