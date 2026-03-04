// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueRow/ChoiceDialogueRow.h"
#include "DialogueWidget/BaseDialogueWidget.h"
#include "ProjectPlayerController.h"
#include "Components/Button.h"
#include "DialogueManager/DialogueManager.h"
#include "DirectingManager/DirectingManager.h"

#include "ChoiceDialogueWidget.generated.h"

class UButton;
class ADialogueManager;
class AProjectPlayerController;
/**
 * 
 */
UCLASS()
class PROJECT_API UChoiceDialogueWidget : public UBaseDialogueWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetChoiceBaseSetting();

	UFUNCTION(BlueprintCallable, Category = "Dialogue|Choice")
	void PlayerChoiceSelect(int _num);

	UPROPERTY(VisibleAnywhere, Category = "DialogueManager")
	AProjectPlayerController* ProjectPlayerController;

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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock4_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock4_2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock4_3;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock4_4;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock3_1;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock3_2;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock3_3;
public:
	void HiideFourChocie();
	void HideThreeChoice();
	void HideTwoChoice();
	void VisibilityFourChoice();
	void VisibilityThreeChoice();
	void VisibilityTwoChoice();

	void SetTwoChoice();
	void SetThreeChoice();
	void SetFourChoice();
private:

	UPROPERTY()
	FName NextID;

	const FChoiceDialogueRow* ChoiceRow;

	UPROPERTY()
	ADirectingManager* DirectingManager;

};
