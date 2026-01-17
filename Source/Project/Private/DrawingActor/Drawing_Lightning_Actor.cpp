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
    TArray<AActor*> ElectricalActors;

    FCollisionShape Sphere = FCollisionShape::MakeSphere(100.f); // 범위 조절 가능
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신 무시


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
            if (HitActor->Tags.Contains(FName("Electrical")))
            {
                ElectricalActors.AddUnique(HitActor);
            }
        }
    }


    for (AActor* EleActor : ElectricalActors)
    {
        if (AElectricalDevice* Device = Cast<AElectricalDevice>(EleActor))
        {
            Device->Use_Function();
        }
        else
        {
            // Actor Tags는 Electrical인데 C++ 클래스가 아닐 수 있음
            UE_LOG(LogTemp, Warning, TEXT("%s has 'Electrical' tag but is not an ElectricalDevice!"), *EleActor->GetName());
        }
    }


    Destroy();
}

