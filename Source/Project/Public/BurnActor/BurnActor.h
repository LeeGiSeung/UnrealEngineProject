// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "BurnActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ABurnActor : public ABaseActor
{
	GENERATED_BODY()
	
public:
	ABurnActor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void SetIsBurn();

	bool GetIsBurn();

	float CurTime;

	bool IsBurn;

	UPROPERTY(BlueprintReadWrite, Category = "BurnTime")
	float BurnTime;

};
