// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DialogueManager.generated.h"

UCLASS()
class PROJECT_API ADialogueManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADialogueManager();
protected:
	virtual void BeginPlay() override;

public:	
	void StartDialogue(FName name);
	void ShowCurDialogue();

};
