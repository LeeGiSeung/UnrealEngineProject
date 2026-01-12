// Fill out your copyright notice in the Description page of Project Settings.


#include "DrawingActor/Drawing_Water_Actor.h"



ADrawing_Water_Actor::ADrawing_Water_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}
