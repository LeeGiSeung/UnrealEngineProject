// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Earthquake/Earthquake.h"

AEarthquake::AEarthquake()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEarthquake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEarthquake::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DeleteEarthquake, this, &AEarthquake::OnEarthquakeTimerFinished, 5.f, false);
}

void AEarthquake::OnEarthquakeTimerFinished()
{
	Destroy();
}
