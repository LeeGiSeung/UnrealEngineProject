// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TogetherManager/TogetherManager.h"
#include "NPC/TogetherRun/TogetherRunBase.h"

void UTogetherManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTogetherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UTogetherManager::AddChainArray(ATogetherRunBase* value)
{
	if (ChainArray.Contains(value)) return;

	ChainArray.Add(value);
}

int UTogetherManager::GetChainArrayIndex()
{
	return ChainArray.Num();
}

TArray<ATogetherRunBase*> UTogetherManager::GetChainArray()
{
	return ChainArray;
}

