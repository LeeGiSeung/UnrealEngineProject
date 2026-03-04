// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawingBaseActor.h"
#include "DrawingRandomActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ADrawingRandomActor : public ADrawingBaseActor
{
	GENERATED_BODY()

public:
	ADrawingRandomActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

};
