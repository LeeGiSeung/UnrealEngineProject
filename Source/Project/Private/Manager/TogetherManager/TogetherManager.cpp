// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TogetherManager/TogetherManager.h"


void UTogetherManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTogetherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UTogetherManager::SetChainArray(TArray<AActor*> value)
{
	ChainArray = value;
}

TArray<AActor*> UTogetherManager::GetChainArray()
{
	return ChainArray;
}

