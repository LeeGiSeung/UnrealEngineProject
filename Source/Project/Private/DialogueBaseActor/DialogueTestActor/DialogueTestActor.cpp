// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueBaseActor/DialogueTestActor/DialogueTestActor.h"

ADialogueTestActor::ADialogueTestActor()
{
	DialogueName = "ID_NORMAL_0";
	StartUIType = EDialogueUIType::Normal;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(RootComponent);

}
