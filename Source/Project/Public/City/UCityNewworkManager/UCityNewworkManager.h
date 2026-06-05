// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "City/CitySystemStruct/CitySystemStruct.h"

#include "UCityNewworkManager.generated.h"

class AABuildingBase;
class ARoadActor;
class AProjectCharacter;

/**
 * 
 */
UCLASS()
class PROJECT_API UUCityNewworkManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadBuildingDataAsset(bool& retFlag);

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<AABuildingBase> BuildingBase;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<ARoadActor> RoadActorClass;

	UPROPERTY()
	float BuildingBetweenDistance;

	void BuildNavigationNetwork();
	
	UFUNCTION()
	TArray<FRoadNode> Navigation(AProjectCharacter* player ,const FVector PlayerLocation);

	UPROPERTY()
	int32 maxNodeCount = 1e9;

	FRoadNode SelectNode;

	TArray<FRoadNode> DfsNavigation(int NodeID, int NodeCount, int GoalNodeID);

	UPROPERTY(EditAnywhere, Category = "Navigation")
	TSubclassOf<AActor> DebugBlockClass;

private:

	void LoadQGIS();

	void LoadRoad(bool& retFlag);

	void LoadBuilding(bool& retFlag);

	float standX = -1400.f;
	float standY = -500.f;

	TArray<FRoadNode> Nodes;
	TArray<FRoadEdge> Edges;

	TArray<ARoadActor*> OutRoadVector;
	
};
