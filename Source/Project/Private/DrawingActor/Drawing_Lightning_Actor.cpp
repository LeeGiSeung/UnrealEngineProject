// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Lightning_Actor.h"



ADrawing_Lightning_Actor::ADrawing_Lightning_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ADrawing_Lightning_Actor::Tick(float DeltaTime)
{
}

void ADrawing_Lightning_Actor::UseAbility()
{
	UE_LOG(LogTemp, Warning, TEXT("Lighting"));
}
