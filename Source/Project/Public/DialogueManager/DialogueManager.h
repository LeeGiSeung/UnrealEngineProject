// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FDialogueRow/FDialogueRow.h"
#include "DialogueManager.generated.h"

class UNormalDialogueWidget;
class UBaseDialogueWidget;

UCLASS()
class PROJECT_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogueManager();
protected:
	virtual void BeginPlay() override;

public:	
	void StartDialogue(FName _ID);
	void ShowCurDialogue();
	void NextDialogue();

	void RemoveCurDialogueWidget();
	void ChangeCurDialogueWidgetText();
	void EndDialogue();

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	UDataTable* DialogueTable;

	bool GetUseDialogue();
	void SetUseIdalogue(bool _value);

protected:
	UPROPERTY()
	EDialogueUIType UIType;

	UPROPERTY()
	FName ID;

	UPROPERTY()
	FName NextID;

	UPROPERTY()
	FName EventKey;

private: //DialogueWidgetList

	UPROPERTY(EditAnywhere, Category = "CurWidget")
	UBaseDialogueWidget* CurDialogueWidget;

	UPROPERTY(EditAnywhere, Category = "DialogueWidget")
	TMap< EDialogueUIType, TSubclassOf<UBaseDialogueWidget>> DialogueWidgetMap;

	bool bDialogue = false;

	const FDialogueRow* Row;
};
