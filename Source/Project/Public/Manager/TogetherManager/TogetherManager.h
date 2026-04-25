// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TogetherManager.generated.h"

class ATogetherRunBase;

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
	TArray<ATogetherRunBase*> ChainArray;

	void AddChainArray(ATogetherRunBase* value);
	int GetChainArrayIndex();
	TArray<ATogetherRunBase*> GetChainArray();

};
