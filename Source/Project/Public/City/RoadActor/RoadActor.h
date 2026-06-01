// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RoadActor.generated.h"

class USplineComponent;

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

	UPROPERTY(VisibleAnywhere, Category = "SplineMesh")
	USplineComponent* RoadSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Road")
	UStaticMesh* RoadMesh;

	void SpawnRoadActor(TArray<FVector> a, double b);

	void SetWorldPoints(TArray<FVector> value);

	TArray<FVector> WorldPoints;
private:
	

};
