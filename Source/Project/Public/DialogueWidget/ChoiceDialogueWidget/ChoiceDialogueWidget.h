// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueWidget/BaseDialogueWidget.h"
#include "ChoiceDialogueWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class PROJECT_API UChoiceDialogueWidget : public UBaseDialogueWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UButton* Button1;

	UPROPERTY(meta = (BindWidget))
	UButton* Button2;

	UPROPERTY(meta = (BindWidget))
	UButton* Button3;

	UPROPERTY(meta = (BindWidget))
	UButton* Button4;
};
