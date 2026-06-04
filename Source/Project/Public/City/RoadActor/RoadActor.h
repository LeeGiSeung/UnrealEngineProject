// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineMeshComponent.h"

#include "RoadActor.generated.h"

class USplineComponent;
class USplineMeshComponent;

UCLASS()
class PROJECT_API ARoadActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoadActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "SplineComponent")
	USplineComponent* RoadSpline;

	UPROPERTY(BlueprintReadOnly,VisibleAnywhere, Category = "SplineMesh")
	USplineMeshComponent* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road")
	UStaticMesh* RoadMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Road")
	double RoadWidth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Road")
	int32 RoadCount;

	UPROPERTY(BlueprintReadWrite, Category = "Navigation")
	TArray<USplineMeshComponent*> SplineMeshArray;

	void SpawnRoadActor(TArray<FVector> a, int32 RoadCount ,double b);

	void SetWorldPoints(TArray<FVector> value);

	TArray<FVector> WorldPoints;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	TArray<class USplineMeshComponent*> SpawnedMeshes;

	UFUNCTION()
	void ChangeRoadColor(int SegIdx, FColor Color);

	UPROPERTY(BlueprintReadWrite, Category = "Navigation")
	TSubclassOf<AActor> DebugBlockClass;

private:
	

};
