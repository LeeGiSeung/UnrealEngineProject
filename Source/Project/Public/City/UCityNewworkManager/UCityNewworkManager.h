// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "UCityNewworkManager.generated.h"

class AABuildingBase;
class ARoadActor;
class AProjectCharacter;

struct FRoadNode {
	int32 NodeID;
	FVector Location;

	// 이 노드에 연결된 무방향 에지들의 ID 목록
	TArray<int32> ConnectingEdgeIDs;
};

struct FRoadEdge {
	int32 EdgeID;

	// Start/End 대신 무방향을 뜻하는 NodeA, NodeB 사용
	int32 NodeA;
	int32 NodeB;

	float Distance;

	// 이 에지가 어떤 도로의 몇 번째 세그먼트인지 저장
	class ARoadActor* OwnerRoadActor;
	int32 SegmentIndex;
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
	
	UFUNCTION()
	void Navigation(AProjectCharacter* player ,const FVector PlayerLocation);

	UFUNCTION(BlueprintCallable)
	void TestNavigation(int32 value);

	UPROPERTY()
	int32 maxNodeCount = 1e9;

	FRoadNode* SelectNode;

	TArray<FRoadNode>* DfsNavigation(int NodeID, int NodeCount, int GoalNodeID);

	void ChangeNodeColor(FRoadNode& Node, FLinearColor color);

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
