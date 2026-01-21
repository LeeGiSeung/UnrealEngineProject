// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawingBaseActor.h"
#include "Drawing_Fire_Actor.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

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

	bool AreAllBurnActorsFinished();

	UPROPERTY(BlueprintReadWrite, Category = "Fire")
	UNiagaraSystem* FireNiagaraEffect;

	UPROPERTY(BlueprintReadWrite, Category = "Fire")
	float FireTime = 20.f;
	FHitResult hit;

	TArray<AActor*> BurnActors;
	UNiagaraComponent* FireComp;

	FVector f;

	float CurTime = 0.f;

	float BurnLimitTime = 3.f;

	bool bUseAbility;

protected:
	virtual void BeginPlay() override;

};
