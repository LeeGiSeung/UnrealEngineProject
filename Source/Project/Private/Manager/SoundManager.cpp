// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundRow/SoundRow.h"
#include "SoundGameInstance/SoundGameInstance.h"

void USoundManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	

	UE_LOG(LogTemp, Warning, TEXT("CastTable"));
}

void USoundManager::PlaySFX()
{
}

void USoundManager::PlayDialogueSound(FName ID)
{
	USoundGameInstance* GI = Cast<USoundGameInstance>(GetGameInstance());
	if (!GI)
	{
		UE_LOG(LogTemp, Error, TEXT("GameInstance is NOT USoundGameInstance"));
		return;
	}

	SoundTable = GI->GetSoundTable();
	
	if (!SoundTable) {
		UE_LOG(LogTemp, Error, TEXT("SoundTable is NOT"));
		return;
	}

	FSoundRow* SoundRow = SoundTable->FindRow<FSoundRow>(ID, TEXT("No SoundTable"));

}

void USoundManager::PlayBGM()
{
}
