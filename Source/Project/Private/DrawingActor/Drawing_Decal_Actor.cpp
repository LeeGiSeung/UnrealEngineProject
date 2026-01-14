// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Decal_Actor.h"
#include "ProjectPlayerController.h"

ADrawing_Decal_Actor::ADrawing_Decal_Actor() {

}

void ADrawing_Decal_Actor::BeginPlay()
{
    if (AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        PC->RegisterDrawingDecar(this);
    }
}

void ADrawing_Decal_Actor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        PC->UnregisterDrawingDecar(this);
    }

    Super::EndPlay(EndPlayReason);
}
