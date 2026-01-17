// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Lightning_Actor.h"

#include "Components/BoxComponent.h"
#include "etc/GameplayTag.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "ElectricalDevice/ElectricalDevice.h"

ADrawing_Lightning_Actor::ADrawing_Lightning_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ADrawing_Lightning_Actor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

void ADrawing_Lightning_Actor::BeginPlay()
{
    Super::BeginPlay();

}

void ADrawing_Lightning_Actor::UseAbility()
{
    TArray<FOverlapResult> Overlaps;
    TArray<AActor*> ElectrialActor;

    FCollisionShape Sphere = FCollisionShape::MakeSphere(100);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    bool bHit = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Pawn,
        Sphere,
        Params
    );

    if (bHit)
    {
        for (const FOverlapResult& Result : Overlaps)
        {
            AActor* HitActor = Result.GetActor();
            if (HitActor)
            {
                ElectrialActor.AddUnique(HitActor);
                UE_LOG(LogTemp, Warning, TEXT("Found: %s"), *HitActor->GetName());
            }
        }
    }

    for (AActor* EleActor : ElectrialActor) {
        Cast<AElectricalDevice>(EleActor)->Use_Function();
    }

    Destroy(); //본인 바로 삭제

}
