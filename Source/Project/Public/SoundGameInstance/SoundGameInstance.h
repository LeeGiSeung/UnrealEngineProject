// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SoundGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API USoundGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UDataTable* SoundTable;

	const UDataTable* GetSoundTable() { return SoundTable; }

};
