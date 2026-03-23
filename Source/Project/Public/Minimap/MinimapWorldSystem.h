// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MinimapWorldSystem.generated.h"

class UTickableWorldSubsystem;

UENUM(Blueprinttype)
enum class EnumMinimapType : uint8 
{
	Road, //길
	Shop, //상점
	Boss, //보스
	Mineral, //광물
	End
};

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
class PROJECT_API UMinimapWorldSystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

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
