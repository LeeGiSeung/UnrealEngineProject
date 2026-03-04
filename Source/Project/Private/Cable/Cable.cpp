// Fill out your copyright notice in the Description page of Project Settings.


#include "Cable/Cable.h"

#include "CableComponent.h"
#include "Components/BoxComponent.h"

ACable::ACable() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(Root);

	Cable->CableLength = 300.f;
	Cable->NumSegments = 10;
	Cable->CableWidth = 5.f;
}

void ACable::BeginPlay()
{
	Super::BeginPlay();
}

void ACable::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}