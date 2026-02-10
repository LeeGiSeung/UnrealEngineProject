// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueWidget/BaseDialogueWidget.h"
#include "DialogueManager/DialogueManager.h"
#include "AutoDialogueRow/AutoDialogueRow.h"
#include "AutoDialogueWidget.generated.h"

class ADialogueManager;

/**
 * 
 */
UCLASS()
class PROJECT_API UAutoDialogueWidget : public UBaseDialogueWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	float CurAutoTime = 0.f;
	float TextTime = 0.f;


	const FAutoDialogueRow* AutoRow;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DialogueManager")
	ADialogueManager* DialogueManager;

	UFUNCTION(BlueprintCallable)
	void CheckAutoDialogueTime(float DeltaTime);

	//virtual void EventTick() override;

};
