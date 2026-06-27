// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "TogetherManager.generated.h"

class ATogetherRunBase;
class AProjectCharacter;

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

	void AddChainArray(ATogetherRunBase* value);
	void RemoveChainArray(ATogetherRunBase* value);
	void RegisterPlayer(AProjectCharacter* value);
	void PlaceChainArray(const float speed, const FVector HandLocation);

	void RegisterTogetherActor(ATogetherRunBase* value);
	TArray<ATogetherRunBase*>* GetWorldSpawnChainArray();

	int GetChainArrayNum();

	
	TArray<ATogetherRunBase*> GetChainArray();
	TArray<ATogetherRunBase*>& GetChainReferenceArray();

private:
	TArray<ATogetherRunBase*> ChainArray;
	TArray<ATogetherRunBase*> WorldSpawnChainArray;

	AProjectCharacter* Player;

	FName TogetherNPCLeftHandName = "hand_l_Socket";
	FName TogetherNPCRIghtHandName = "hand_r_Socket";

};
