// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectingManager/DirectingManager.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieScene.h"
#include "MovieSceneObjectBindingID.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "MovieSceneBinding.h"
#include "MovieSceneObjectBindingID.h"



ADirectingManager::ADirectingManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADirectingManager::BeginPlay()
{
	Super::BeginPlay();
	
    PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();

}

void ADirectingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADirectingManager::PlayEvent(FName DirectingKey, EDialogueUIType value)
{
    if (!EventTable) return;

	Row = EventTable->FindRow<FEventRow>(DirectingKey, TEXT("NO FIND EVENT KEY"));

    if (!Row) return;

    UIType = value;
    LoopFrame = Row->LoopFrame; //LoopFrame
    SetLevelSequencePlay(false);
    PlayLevelSequence(Row->LevelSequence);

}

void ADirectingManager::PlayLevelSequence(ULevelSequence* Sequence)
{
    ALevelSequenceActor* SeqActor = nullptr;

    SequencePlayer =
        ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            Sequence,
            FMovieSceneSequencePlaybackSettings(),
            SeqActor
        );

    if (!SequencePlayer || !SeqActor)
        return;

    for (const auto& Pair : SequenceTagMap)
    {
        if (AActor* FoundActor = Pair.Value.Get())
        {
            TArray<AActor*> Actors;
            Actors.Add(FoundActor);

            SeqActor->SetBindingByTag(Pair.Key, Actors, false);
        }
    }

    SequencePlayer->Play();
}

ADirectingManager* ADirectingManager::GetDirectingManager(UWorld* World)
{
    return Cast<ADirectingManager>(
        UGameplayStatics::GetActorOfClass(World, ADirectingManager::StaticClass())
    );
}

void ADirectingManager::SetLevelSequencePlay(bool _value)
{
    if (!SequencePlayer) return;

    bLevelSequencePlay = _value;
    
    if (bLevelSequencePlay == true) { //끝났으면
        SequencePlayer->Stop();
    }
}

bool ADirectingManager::GetLevelSequencePlay()
{
    return bLevelSequencePlay;
}

void ADirectingManager::EndLevelSequence()
{

    if (!SequencePlayer) return;

    if (UIType == EDialogueUIType::Auto) return; //auto면 반복할 필요 없음

    if (!GetLevelSequencePlay()) {
        FMovieSceneSequencePlaybackParams Params;
        Params.Frame = FFrameTime(LoopFrame);
        Params.PositionType = EMovieScenePositionType::Frame;

        SequencePlayer->SetPlaybackPosition(Params);
    }    
}

void ADirectingManager::SetNextFrame(bool _value)
{
    bNextFrame = _value;
}

bool ADirectingManager::GetNextFrame()
{
    return bNextFrame;
}

void ADirectingManager::RegisterSequenceActor(FName Tag, AActor* Actor)
{
    if (!Actor)
    {
        UE_LOG(LogTemp, Warning, TEXT("RegisterSequenceActor: Actor is null"));
        return;
    }

    SequenceTagMap.Add(Tag, Actor);

    UE_LOG(LogTemp, Log, TEXT("Registered %s with tag %s"),
        *Actor->GetName(),
        *Tag.ToString());
}



