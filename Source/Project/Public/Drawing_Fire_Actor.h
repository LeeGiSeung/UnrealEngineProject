// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawingBaseActor.h"
#include "Drawing_Fire_Actor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ADrawing_Fire_Actor : public ADrawingBaseActor
{
	GENERATED_BODY()
	
public:
	ADrawing_Fire_Actor();
	virtual void UseAbility() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

};
