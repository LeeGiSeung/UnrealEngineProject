// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueBaseActor/DialogueBaseActor.h"
#include "DialogueTestActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ADialogueTestActor : public ADialogueBaseActor
{
	GENERATED_BODY()
	
public:
	ADialogueTestActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComp;

};
