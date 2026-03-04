// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DialogueType/DialogueType.h"
#include "DialogueManager/DialogueManager.h"

#include "SoundManager.generated.h"

class USoundGameInstance;
/**
 * 
 */
UCLASS()
class PROJECT_API USoundManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void PlaySFX();
	void PlayBGM();
	void StopDialogueSound();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;



	UPROPERTY()
	const UDataTable* SoundTable;

	UPROPERTY()
	ADialogueManager* DialogueManager;

//CALLBACKFUNCTION
public:
	UFUNCTION()
	void OnFinishedPlayDialogueSound();

//AutoDialogue
public:
	void PlayDialogueSound(FName ID, EDialogueUIType type);

	FTimerHandle SoundDelayHandle;

	EDialogueUIType CurType;

private:
	UAudioComponent* AudioComp;
};
