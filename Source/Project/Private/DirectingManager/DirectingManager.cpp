// Fill out your copyright notice in the Description page of Project Settings.


#include "DirectingManager/DirectingManager.h"

ADirectingManager::ADirectingManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADirectingManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADirectingManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADirectingManager::PlayEvent(FName DirectingKey)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *DirectingKey.ToString());
	return;
}

