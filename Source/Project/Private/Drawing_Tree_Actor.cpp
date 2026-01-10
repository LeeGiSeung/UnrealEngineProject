// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawing_Tree_Actor.h"

ADrawing_Tree_Actor::ADrawing_Tree_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADrawing_Tree_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();
	NewLocation.Z = NewLocation.Z + MoveSpeed * DeltaTime;
	SetActorLocation(NewLocation);
}

void ADrawing_Tree_Actor::BeginPlay()
{
	Super::BeginPlay();
}
