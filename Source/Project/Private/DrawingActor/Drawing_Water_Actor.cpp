// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Water_Actor.h"
#include "WaterBodyCustomActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectPlayerController.h"
#include "Manager/DrawingActorManager.h"
#include "EngineUtils.h"

ADrawing_Water_Actor::ADrawing_Water_Actor()
{
    PrimaryActorTick.bCanEverTick = true;

    RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    WaterNiagaraEffect = nullptr;
}

void ADrawing_Water_Actor::Tick(float DeltaTime)
{

    Super::Tick(DeltaTime);

    if (!bUseAbility) return;
    if (CurTime >= WaterTime)
    {
        DrawingManager->DeleteDrawingActor(this);
        Destroy();
    }
    CurTime += DeltaTime;

    FHitResult WaterHit;

    FVector Start = GetActorLocation() + FVector(0, 0, 50);
    FVector End = Start + FVector(0, 0, -10000);

    AActor* HitActor = nullptr;

    if (GetWorld()->LineTraceSingleByChannel(
        WaterHit,
        Start,
        End,
        ECC_Visibility
    ))
    {
        HitActor = WaterHit.GetActor();

        if (HitActor && HitActor->ActorHasTag("Water"))
        {
            FVector Sc = HitActor->GetActorScale3D();
            Sc.Z += DeltaTime * 0.5f;
            HitActor->SetActorScale3D(Sc);
        }
    }


}

void ADrawing_Water_Actor::BeginPlay()
{
    Super::BeginPlay();
    
    AProjectPlayerController* PC =
        Cast<AProjectPlayerController>(
            UGameplayStatics::GetPlayerController(GetWorld(), 0)
        );

    if (PC) {
        hit = PC->GetHit(); //소환 됐을때의 hit 저장
        f = PC->GetActorSpawnScale();
    }

}

void ADrawing_Water_Actor::UseAbility()
{


    if (WaterNiagaraEffect)
    {
        FVector CurLocation = GetActorLocation();

        FVector Normal = hit.ImpactNormal;

        constexpr float SurfaceThreshold = 0.7f;

        // 표면 법선 기준 회전
        FRotator SelectedSpawnRotation = UKismetMathLibrary::MakeRotFromZ(Normal);

        //CurLocation.Z += 100; //이것도 나중에 유동적 사이즈 되면 수정할거
        FVector Forward = GetActorForwardVector();

        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            WaterNiagaraEffect,
            hit.ImpactPoint,
            SelectedSpawnRotation,
            f,                // 유동적 크기
            true,             // AutoDestroy
            true              // AutoActivate
        );

        bUseAbility = true;
    }
}
