// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyManager/EnemyManager.h"
#include "Enemy/BaseEnemy/BaseEnemy.h"

void UEnemyManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UEnemyManager::RegisterEnemyToManager(ABaseEnemy* Value)
{
	if (!Value) return;

	EnemyManagerArray.Add(Value);
	OnRegisterEnemyToManager.Broadcast();
}

int32 UEnemyManager::GetEnemyManagerArrayNum()
{
	return EnemyManagerArray.Num();
}

TArray<ABaseEnemy*>* UEnemyManager::GetEnemyManagerArray()
{
	return &EnemyManagerArray;
}
