// Fill out your copyright notice in the Description page of Project Settings.


#include "Minimap/MinimapWorldSystem.h"
#include "Subsystems/WorldSubsystem.h"

void UMinimapWorldSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UMinimapWorldSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TStatId UMinimapWorldSystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMinimapWorldSystem, STATGROUP_Tickables);
}

void UMinimapWorldSystem::RegisterMakerData(AActor* actor, EnumMinimapType MarkerType)
{
	if (!actor) return;
	
	ActiveMarkers.Add(FMinimapMakerData(actor, MarkerType));
}

void UMinimapWorldSystem::UnRegisterMakerData(AActor* actor, EnumMinimapType MarkerType)
{
	if (!actor) return;

	ActiveMarkers.Remove(FMinimapMakerData(actor, MarkerType));
}

TArray<FMinimapMakerData> UMinimapWorldSystem::GetActiveMarkers()
{
	return ActiveMarkers;
}
