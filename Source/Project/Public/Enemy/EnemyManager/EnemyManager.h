// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManager.generated.h"

class ABaseEnemy;

DECLARE_MULTICAST_DELEGATE(FOnRegisterEnemyToManager);

/**
 * 
 */
UCLASS()
class PROJECT_API UEnemyManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaTime) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual TStatId GetStatId() const override {
		RETURN_QUICK_DECLARE_CYCLE_STAT(UEnemyManager, STATGROUP_Tickables);
	}

public:
	void RegisterEnemyToManager(ABaseEnemy* Value);
	int32 GetEnemyManagerArrayNum();

	TArray<ABaseEnemy*>* GetEnemyManagerArray();

	FOnRegisterEnemyToManager OnRegisterEnemyToManager;

private:
	TArray<ABaseEnemy*> EnemyManagerArray;

	
};
