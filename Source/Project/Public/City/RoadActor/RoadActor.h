// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "RoadActor.generated.h"

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

	UPROPERTY(VisibleAnywhere, Category = "SplineMesh")
	USplineMeshComponent* SplineMesh;

};
