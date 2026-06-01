// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UCityNewworkManager.generated.h"

class AABuildingBase;
class ARoadActor;

struct FRoadNode {
	int32 NodeID;    //노드ID
	FVector Location; //노드위치
	TArray<int32> ConnectingEdgeID; //붙어있는 EdgeID
};

struct FRoadEdge {
	int32 EdgeID;
	int32 StartNodeID;
	int32 EndNodeID;
	float Distance;

	TWeakObjectPtr<ARoadActor> OwnerRoadActor;
	int32 SegmentIndex; // 이 간선이 해당 스플라인의 몇 번째 구간인지
};


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
