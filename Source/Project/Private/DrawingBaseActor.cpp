// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingBaseActor.h"
#include "ProjectPlayerController.h"

ADrawingBaseActor::ADrawingBaseActor()
{
}

void ADrawingBaseActor::BeginPlay()
{
    Super::BeginPlay();

    if (AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        PC->RegisterDrawingActor(this);
    }
}

void ADrawingBaseActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        PC->UnregisterDrawingActor(this);
    }

    Super::EndPlay(EndPlayReason);
}
