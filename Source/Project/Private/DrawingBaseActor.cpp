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

void ADrawingBaseActor::UseAbility()
{
    //UE_LOG(LogTemp, Warning, TEXT("BASEACTOR"));
    //자세한 능력은 각 Actor에서 실행
}

void ADrawingBaseActor::SetHighlight(bool bEnable)
{
    if (!RenderMesh) return;

    RenderMesh->SetRenderCustomDepth(bEnable);
    RenderMesh->SetCustomDepthStencilValue(1); // 1번 ID
}
