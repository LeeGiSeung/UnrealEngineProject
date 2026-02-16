// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundRow/SoundRow.h"
#include "EngineUtils.h"

#include "SoundGameInstance/SoundGameInstance.h"

void USoundManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void USoundManager::PlaySFX()
{
}

void USoundManager::PlayDialogueSound(FName ID, EDialogueUIType type)
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

	AudioComp = UGameplayStatics::SpawnSound2D(this, SoundRow->Sound);

	if (AudioComp) {
		CurType = type;
		AudioComp->OnAudioFinished.AddDynamic(this,&ThisClass::OnFinishedPlayDialogueSound);
	}

}

void USoundManager::PlayBGM()
{
}

void USoundManager::StopDialogueSound()
{
	if (AudioComp && AudioComp->IsPlaying()) {
		AudioComp->Stop();
	}
}

void USoundManager::OnFinishedPlayDialogueSound()
{
	if (CurType != EDialogueUIType::Auto) return;
	//Normal, Choice는 현재 npc의 음성과 관계없이 선택을 해야 넘어가지만, 
	//Auto는 사람 말이 끝나야 넘어가는게 있음

	for (TActorIterator<ADialogueManager> It(GetWorld()); It; ++It)
	{
		DialogueManager = *It;
		break;
	}

	check(DialogueManager); // 없으면 바로 알 수 있게

	GetWorld()->GetTimerManager().SetTimer(
		SoundDelayHandle,
		DialogueManager,
		&ADialogueManager::NextNormalDialogue,
		1.0f,      // 딜레이 시간
		false      // 반복 아님
	);
}