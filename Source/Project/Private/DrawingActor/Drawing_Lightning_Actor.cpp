// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Lightning_Actor.h"

#include "Components/BoxComponent.h"
#include "etc/GameplayTag.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"
#include "ElectricalDevice/ElectricalDevice.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Manager/DrawingActorManager.h"
#include "EngineUtils.h"

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

    FCollisionShape Sphere = FCollisionShape::MakeSphere(300.f); // 범위 조절 가능
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // 자기 자신 무시

    //DrawDebugSphere(
    //    GetWorld(),
    //    GetActorLocation(),
    //    Sphere.GetSphereRadius(),
    //    32,               // 세그먼트 수
    //    FColor::Green,    // 색상
    //    false,            // 지속 시간 무한 아님
    //    2.f,              // 지속 시간
    //    0,                // 깊이 우선 표시
    //    2.f               // 선 두께
    //);

    bool bHit = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        GetActorLocation(),
        FQuat::Identity,
        ECC_Visibility, // 필요 시 ObjectType 조정 가능
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
    else {
        UE_LOG(LogTemp, Warning, TEXT("No Dict"));
    }


    for (AActor* EleActor : ElectricalActors)
    {
        if (AElectricalDevice* Device = Cast<AElectricalDevice>(EleActor))
        {
            //UE_LOG(LogTemp, Warning, TEXT("%s Use ElectricalDevice!"), *EleActor->GetName());
            Device->Use_Function();
        }
        else
        {
            // Actor Tags는 Electrical인데 C++ 클래스가 아닐 수 있음
            UE_LOG(LogTemp, Warning, TEXT("%s has 'Electrical' tag but is not an ElectricalDevice!"), *EleActor->GetName());
        }
    }

    DrawingManager->DeleteDrawingActor(this);
    Destroy();
    
}

