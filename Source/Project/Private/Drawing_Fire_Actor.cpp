// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawing_Fire_Actor.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectPlayerController.h"


ADrawing_Fire_Actor::ADrawing_Fire_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    FireNiagaraEffect = nullptr;
}

void ADrawing_Fire_Actor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bUseAbility) return;
    if (CurTime >= FireTime) Destroy();

    CurTime += DeltaTime;

 
}

void ADrawing_Fire_Actor::BeginPlay()
{
    Super::BeginPlay();

    AProjectPlayerController* PC =
        Cast<AProjectPlayerController>(
            UGameplayStatics::GetPlayerController(GetWorld(), 0)
        );

    if (PC) hit = PC->GetHit(); //소환 됐을때의 hit 저장
}

void ADrawing_Fire_Actor::UseAbility()
{
    if (FireNiagaraEffect)
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
            FireNiagaraEffect,
            hit.ImpactPoint,
            SelectedSpawnRotation,
            FVector(1.f),     // Scale
            true,             // AutoDestroy
            true              // AutoActivate
        );

        bUseAbility = true;
    }

    RenderMesh->SetVisibility(false);      // 화면에 안 보이게
    RenderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌도 없게
    RenderMesh->SetHiddenInGame(true);

}
