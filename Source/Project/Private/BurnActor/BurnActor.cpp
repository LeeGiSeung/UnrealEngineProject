// Fill out your copyright notice in the Description page of Project Settings.


#include "BurnActor/BurnActor.h"

ABurnActor::ABurnActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABurnActor::BeginPlay()
{
	Super::BeginPlay();
}

void ABurnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsBurn) {
		CurTime += DeltaTime;

		UE_LOG(LogTemp, Warning, TEXT("%f"), CurTime);

		if (CurTime >= BurnTime) {
			Destroy();
		}
	}
}

void ABurnActor::SetIsBurn()
{
	IsBurn = true;
}

bool ABurnActor::GetIsBurn()
{
	return IsBurn;
}
