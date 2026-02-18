// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FDialogueRow/FDialogueRow.h"
#include "AutoDialogueRow/AutoDialogueRow.h"
#include "DialogueRow/ChoiceDialogueRow.h"
#include "ProjectPlayerController.h"
#include "DirectingManager/DirectingManager.h"
#include "DialogueManager.generated.h"

class UBaseDialogueWidget;
class UNormalDialogueWidget;
class UChoiceDialogueWidget;
class UAutoDialogueWidget;
class USoundManager;

UCLASS()
class PROJECT_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogueManager();
protected:
	virtual void BeginPlay() override;

public:	
	void StartDialogue(FName _ID, EDialogueUIType _Type);
	void ShowCurDialogue();
	void PlayerChoiceNumberCheck();
	void WidgetAddViewPort();
	void NextNormalDialogue();
	void NextChoiceDialogue();
	

	void ResetCurDialogueWidget();
	void ChangeCurDialogueWidgetNormalText();
	void ChangeCurDialogueWidgetChoiceText();
	void ChangeCurDialogueWidgetAutoText();
	void ChangeCurDialogueWidgetChoice();
	void EndDialogue();
	void SaveAndRemoveAllWidgets();
	void ShowAllWidget();

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	UDataTable* DialogueNormalTable;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	UDataTable* DialogueChoiceTable;

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	UDataTable* DialogueAutoTable;

	bool GetUseDialogue();
	void SetUseIdalogue(bool _value);

	const FDialogueRow* GetNormalRow() { return NormalRow; }
	const FChoiceDialogueRow* GetChoiceRow() { return ChoiceRow; };
	const FAutoDialogueRow* GetAutoRow() { return AutoRow; };

	EDialogueUIType GetUIType() {return CurUIType;}

protected:
	UPROPERTY()
	EDialogueUIType UIType;

	UPROPERTY()
	EDialogueUIType CurUIType;

	UPROPERTY()
	FName ID;

	UPROPERTY()
	FName NextID;

	UPROPERTY()
	EDialogueUIType NextUIType;

	UPROPERTY()
	FName EventKey;

private: //DialogueWidgetList

	UPROPERTY(EditAnywhere, Category = "CurWidget")
	UBaseDialogueWidget* CurDialogueWidget;

	UPROPERTY()
	UNormalDialogueWidget* NormalWidget;
	UPROPERTY()
	UChoiceDialogueWidget* ChoiceWidget;
	UPROPERTY()
	UAutoDialogueWidget* AutoWidget;

	UPROPERTY(EditAnywhere, Category = "DialogueWidget")
	TMap< EDialogueUIType, TSubclassOf<UBaseDialogueWidget>> DialogueWidgetMap;

	bool bDialogue = false;

	const FDialogueRow* NormalRow;
	const FChoiceDialogueRow* ChoiceRow;
	const FAutoDialogueRow* AutoRow;

	UPROPERTY()
	USoundManager* SoundManager;

	UPROPERTY()
	int iPlayerChoiceNumber;

	UPROPERTY(VisibleAnywhere, Category = "DialogueManager")
	AProjectPlayerController* ProjectPlayerController;

	UPROPERTY()
	TArray<UUserWidget*> StoredWidgets;

	UPROPERTY()
	ADirectingManager* DirectingManager;
};
