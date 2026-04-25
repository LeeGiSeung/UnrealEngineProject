// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TogetherManager.generated.h"



/**
 * 
 */
UCLASS()
class PROJECT_API UTogetherManager : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UTogetherManager, STATGROUP_Tickables);
	}


//TogetherBase Array
public:
	TArray<AActor*> ChainArray;

	void SetChainArray(TArray<AActor*> value);
	TArray<AActor*> GetChainArray();

};
