// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawingBaseActor.h"
#include "Drawing_Tree_Actor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ADrawing_Tree_Actor : public ADrawingBaseActor
{
	GENERATED_BODY()

public:
	ADrawing_Tree_Actor();

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, Category = "Drawing_Tree")
	float MoveSpeed = 500.f;

protected:
	virtual void BeginPlay() override;

};
