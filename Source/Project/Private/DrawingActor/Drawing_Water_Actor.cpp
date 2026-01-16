// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Water_Actor.h"
//#include "WaterBodyCustom.h"
#include "Engine/World.h"


ADrawing_Water_Actor::ADrawing_Water_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ADrawing_Water_Actor::Tick(float DeltaTime)
{
}

void ADrawing_Water_Actor::UseAbility()
{
    UWorld* World = GetWorld();
    if (!World) return;

    //AWaterBodyCustom* TestPtr = nullptr;

    //FActorSpawnParameters SpawnParams;
    //SpawnParams.Owner = this;
    //SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    //AWaterBodyCustom* WaterBody = World->SpawnActor<AWaterBodyCustom>(
    //    AWaterBodyCustom::StaticClass(),
    //    GetActorLocation(),
    //    FRotator::ZeroRotator,
    //    SpawnParams
    //);

    //if (WaterBody)
    //{
    //    WaterBody->SetActorScale3D(FVector(1.f));
    //    UE_LOG(LogTemp, Log, TEXT("Water spawned at %s"), *GetActorLocation().ToString());
    //}
    //else
    //{
    //    UE_LOG(LogTemp, Warning, TEXT("Failed to spawn water."));
    //}
}
