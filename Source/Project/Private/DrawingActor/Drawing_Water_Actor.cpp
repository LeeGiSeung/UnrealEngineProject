// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Water_Actor.h"



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
	UE_LOG(LogTemp, Warning, TEXT("Water"));
}
