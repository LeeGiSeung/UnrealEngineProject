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

	UFUNCTION(BlueprintCallable)
	TArray<FRoadNode> GetNavigationCourse();

private:

	void LoadQGIS();
	
	void LoadBuildingDataAsset(bool& retFlag);
	
	void LoadRoad();
	void LoadBuilding();

	void ConstructBuilding();
	void ConstructRoad();

	UPROPERTY()
	TArray<FRoadData> RoadDataList;

	UPROPERTY()
	TArray<FRuntimeRoadData> TotalRoadData;

	//UPROPERTY()
	//TArray<FRuntimeBuildingData> TotalBuildingData;

	TArray<TSharedPtr<FRuntimeBuildingData>> TotalBuildingData;

	UPROPERTY()
	TSet<int32> CurrentNavigationEdges;

	UFUNCTION()
	void UpdateBuildingVisibility(FVector PlayerLocation);

	UFUNCTION()
	void UpdateRoadVisibility(FVector PlayerLocation);

	FTimerHandle VisibilityTimerHandle;

	UPROPERTY()
	double MinComputeDistance = 70000;

	void CheckCityVisibility();

	void DebugingBuildingCheck(FVector PlayerLocation);

	// 도로를 보여줄 반경 (빌딩보다 조금 더 길게 잡는 것을 추천합니다. 약 250미터 = 25000.f)
	const float RoadSpawnRadius = 25000.f;

	float standX = -1400.f;
	float standY = -500.f;

	TArray<FRoadNode> Nodes;
	TArray<FRoadEdge> Edges;

	TArray<ARoadActor*> OutRoadVector;
	
	TArray<FRoadNode> NavigationCourse;
};
