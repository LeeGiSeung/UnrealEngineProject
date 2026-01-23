// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/DrawingRandomActor.h"

ADrawingRandomActor::ADrawingRandomActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ADrawingRandomActor::BeginPlay()
{

}

void ADrawingRandomActor::Tick(float DeltaTime)
{

}
