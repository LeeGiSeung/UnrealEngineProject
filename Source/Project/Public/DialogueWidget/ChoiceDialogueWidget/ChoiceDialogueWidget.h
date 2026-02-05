// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueWidget/BaseDialogueWidget.h"
#include "DialogueManager/DialogueManager.h"

#include "ChoiceDialogueWidget.generated.h"

class UButton;
class ADialogueManager;
/**
 * 
 */
UCLASS()
class PROJECT_API UChoiceDialogueWidget : public UBaseDialogueWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueManager")
	ADialogueManager* DialogueManager;

	UPROPERTY(meta = (BindWidget))
	UButton* Button3_1;

	UPROPERTY(meta = (BindWidget))
	UButton* Button3_2;

	UPROPERTY(meta = (BindWidget))
	UButton* Button3_3;

	UPROPERTY(meta = (BindWidget))
	UButton* Button4_1;

	UPROPERTY(meta = (BindWidget))
	UButton* Button4_2;

	UPROPERTY(meta = (BindWidget))
	UButton* Button4_3;

	UPROPERTY(meta = (BindWidget))
	UButton* Button4_4;
};
