// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingBaseActor.h"
#include "ProjectPlayerController.h"
#include "Components/BoxComponent.h"

#include "Manager/DrawingActorManager.h"
#include "EngineUtils.h"

ADrawingBaseActor::ADrawingBaseActor()
{

}

void ADrawingBaseActor::BeginPlay()
{
    Super::BeginPlay();

    if (AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController()))
    {
        PC->RegisterDrawingActor(this);
        //UE_LOG(LogTemp, Warning, TEXT("Add Reg"));
    }

    DrawingManager = nullptr;

    for (TActorIterator<ADrawingActorManager> It(GetWorld()); It; ++It)
    {
        DrawingManager = *It;
        break;
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
    //RenderMesh->SetCustomDepthStencilValue(1); // 1번 ID
}



void ADrawingBaseActor::SetDecalActor(ADecalActor* _Decal)
{
    Decal = _Decal;
}

ADecalActor* ADrawingBaseActor::GetDecalActor()
{
    return Decal;
}

void ADrawingBaseActor::SetInUse()
{
    bInUse = true;
}
