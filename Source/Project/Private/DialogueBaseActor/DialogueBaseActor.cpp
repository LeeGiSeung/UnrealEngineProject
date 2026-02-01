// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueBaseActor/DialogueBaseActor.h"
#include "DialogueManager/DialogueManager.h"
#include "EngineUtils.h"

// Sets default values
ADialogueBaseActor::ADialogueBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADialogueBaseActor::BeginPlay()
{
	Super::BeginPlay();

    for (TActorIterator<ADialogueManager> It(GetWorld()); It; ++It)
    {
        DialogueManager = *It;
        break;
    }

    check(DialogueManager); // 없으면 바로 알 수 있게

}

// Called every frame
void ADialogueBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADialogueBaseActor::StartDialogue()
{
    DialogueManager->StartDialogue(DialogueName); 
}

