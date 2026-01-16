// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawingBaseActor.h"
#include "Engine/HitResult.h"
#include "Drawing_Water_Actor.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

/**
 * 
 */
UCLASS()
class PROJECT_API ADrawing_Water_Actor : public ADrawingBaseActor
{
	GENERATED_BODY()

public:
	ADrawing_Water_Actor();
	virtual void UseAbility() override;

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "Water")
	UNiagaraSystem* WaterNiagaraEffect;

	FHitResult hit;

	UPROPERTY(BlueprintReadWrite, Category = "Water")
	float WaterTime = 20.f;

	float CurTime = 0.f;

	bool bUseAbility;

};
