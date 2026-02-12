// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectingManager/DirectingManager.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "Kismet/GameplayStatics.h"

ADirectingManager::ADirectingManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADirectingManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADirectingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirectingManager::PlayEvent(FName DirectingKey)
{
    if (!EventTable) return;

	Row = EventTable->FindRow<FEventRow>(DirectingKey, TEXT("NO FIND EVENT KEY"));

    if (!Row) return;

    PlayLevelSequence(Row->LevelSequence);

}

void ADirectingManager::PlayLevelSequence(ULevelSequence* Sequence)
{
    if (!Sequence) return;

    ALevelSequenceActor* SeqActor = nullptr;

    FMovieSceneSequencePlaybackSettings Settings;

    ULevelSequencePlayer* SequencePlayer =
        ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            Sequence,
            Settings,
            SeqActor
        );

    if (SequencePlayer)
    {
        SequencePlayer->Play();
    }
}

