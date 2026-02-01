// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueBaseActor/DialogueBaseActor.h"

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
	
}

// Called every frame
void ADialogueBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

