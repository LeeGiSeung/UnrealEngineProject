// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectingManager/DirectingManager.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieScene.h"
#include "MovieSceneObjectBindingID.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "MovieSceneObjectBindingID.h"

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
    ALevelSequenceActor* SeqActor = nullptr;

    ULevelSequencePlayer* SequencePlayer =
        ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            Sequence,
            FMovieSceneSequencePlaybackSettings(),
            SeqActor
        );

    if (!SequencePlayer || !SeqActor)
    {
        return;
    }

    if (SeqActor)
    {
        // 실제 플레이어 폰 참조 획득
        APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

        if (PlayerPawn)
        {
            TArray<AActor*> Actors;
            Actors.Add(PlayerPawn);

            // 태그를 사용하여 바인딩 수행
            // bAllowBindingsFromAsset가 false인 경우 기존 에셋의 바인딩을 무시함 
            SeqActor->SetBindingByTag("Player", Actors, false);
        }
    }

    SequencePlayer->Play();
}

