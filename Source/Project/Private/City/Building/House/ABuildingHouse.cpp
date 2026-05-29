// Fill out your copyright notice in the Description page of Project Settings.


#include "City/Building/House/ABuildingHouse.h"



void AABuildingHouse::BeginPlay()
{
	Super::BeginPlay();

}

void AABuildingHouse::Tick(float tick)
{
	Super::Tick(tick);

	UE_LOG(LogTemp, Error, TEXT("TEXT Build"));
}
