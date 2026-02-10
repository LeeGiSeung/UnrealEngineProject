// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
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
	void PlayDialogueSound(FName ID);
	void PlayBGM();
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY()
	const UDataTable* SoundTable;
};
