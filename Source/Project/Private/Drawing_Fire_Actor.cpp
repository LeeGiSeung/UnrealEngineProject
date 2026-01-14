// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawing_Fire_Actor.h"

ADrawing_Fire_Actor::ADrawing_Fire_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ADrawing_Fire_Actor::Tick(float DeltaTime)
{
}

void ADrawing_Fire_Actor::BeginPlay()
{
}

void ADrawing_Fire_Actor::UseAbility()
{

}
