// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawing_Fire_Actor.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ProjectPlayerController.h"
#include "BurnActor/BurnActor.h"
#include "Manager/DrawingActorManager.h"
#include "EngineUtils.h"

ADrawing_Fire_Actor::ADrawing_Fire_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

    FireNiagaraEffect = nullptr;
}

void ADrawing_Fire_Actor::Tick(float DeltaTime)
{

    if (!bUseAbility) return;
    Super::Tick(DeltaTime);

    if (BurnActors.Num() == 0) {
        CurTime += DeltaTime;
        if (CurTime > BurnLimitTime) {
            
            DrawingManager->DeleteDrawingActor(this);
            Destroy();
            
            FireComp->Deactivate();
        }
    }

    if (AreAllBurnActorsFinished() && BurnActors.Num() != 0) {
        BurnActors.Empty();
    }

}

void ADrawing_Fire_Actor::BeginPlay()
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

        FireComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            FireNiagaraEffect,
            hit.ImpactPoint,
            SelectedSpawnRotation,
            FVector(f),
            true,
            true
        );

        bUseAbility = true;
    }

    RenderMesh->SetVisibility(false);      // 화면에 안 보이게
    RenderMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌도 없게
    RenderMesh->SetHiddenInGame(true);

    TArray<FOverlapResult> Overlaps;
    BurnActors;

    FCollisionShape Sphere = FCollisionShape::MakeSphere(100.f); // 범위 조절 가능
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신 무시

    DrawDebugSphere(
        GetWorld(),
        GetActorLocation(),
        Sphere.GetSphereRadius(),
        32,               // 세그먼트 수
        FColor::Green,    // 색상
        false,            // 지속 시간 무한 아님
        2.f,              // 지속 시간
        0,                // 깊이 우선 표시
        2.f               // 선 두께
    );

    bool bHit = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Pawn, // 필요 시 ObjectType 조정 가능
        Sphere,
        Params
    );


    if (bHit)
    {
        for (const FOverlapResult& Result : Overlaps)
        {
            AActor* HitActor = Result.GetActor();
            if (!HitActor) continue; // 안전 체크

            // 3-1. Actor Tags 기반 태그 체크
            if (HitActor->Tags.Contains(FName("Burn")))
            {
                BurnActors.AddUnique(HitActor);
            }
        }
    }


    for (AActor* b : BurnActors)
    {
        if (ABurnActor* burnActor = Cast<ABurnActor>(b))
        {
            //UE_LOG(LogTemp, Warning, TEXT("BURN"));
            burnActor->SetIsBurn();
        }
        else
        {
            // Actor Tags는 Electrical인데 C++ 클래스가 아닐 수 있음
            //UE_LOG(LogTemp, Warning, TEXT("%s has 'Electrical' tag but is not an ElectricalDevice!"), *EleActor->GetName());
        }
    }

}

bool ADrawing_Fire_Actor::AreAllBurnActorsFinished()
{
    for (AActor* Burn : BurnActors)
    {
        if (IsValid(Burn) && !Burn->IsActorBeingDestroyed())
        {
            return false;
        }
    }
    return true;
}