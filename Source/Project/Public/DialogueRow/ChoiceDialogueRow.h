// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueType/DialogueType.h"
#include "ChoiceDialogueRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FChoiceDialogueRow : public FTableRowBase {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText FirstText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText SecondText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ChoiceText1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ChoiceText2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ChoiceText3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ChoiceText4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ChoiceTextAnswer1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ChoiceTextAnswer2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ChoiceTextAnswer3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ChoiceTextAnswer4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDialogueUIType UIType;
};
