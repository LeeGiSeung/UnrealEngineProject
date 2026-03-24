// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MinimapType/MinimapType.h"
#include "MinimapWorldSystem.generated.h"

USTRUCT(BlueprintType)
struct FMinimapMakerData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	AActor* MarkerActor;

	UPROPERTY(BlueprintReadOnly)
	EnumMinimapType MarkerType;

public:
	FMinimapMakerData() : MarkerActor(nullptr), MarkerType(EnumMinimapType::End) {}
	FMinimapMakerData(AActor* actor, EnumMinimapType MinimapType) {
		MarkerActor = actor;
		MarkerType = MinimapType;
	}
	
	bool operator==(const FMinimapMakerData& Other) const {
		return MarkerActor == Other.MarkerActor;
	}
};
/**
 * 
 */
UCLASS()
class PROJECT_API UMinimapWorldSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	//virtual void Tick(float DeltaTime) override;
	//virtual TStatId GetStatId() const override;

	UFUNCTION(BlueprintCallable)
	void RegisterMakerData(AActor* actor, EnumMinimapType MarkerType);
	
	UFUNCTION(BlueprintCallable)
	void UnRegisterMakerData(AActor* actor, EnumMinimapType MarkerType);

	UFUNCTION(BlueprintCallable)
	TArray<FMinimapMakerData> GetActiveMarkers();

private:
	UPROPERTY()
	TArray<FMinimapMakerData> ActiveMarkers;
};
