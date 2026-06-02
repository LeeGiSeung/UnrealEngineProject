// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

	void SpawnRoadActor(TArray<FVector> a, int32 RoadCount ,double b);

	void SetWorldPoints(TArray<FVector> value);

	TArray<FVector> WorldPoints;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY()
	TArray<class USplineMeshComponent*> SpawnedMeshes;

	

	UFUNCTION(BlueprintImplementableEvent, Category = "Road")
	void BuildRoadMesh();

private:
	

};
