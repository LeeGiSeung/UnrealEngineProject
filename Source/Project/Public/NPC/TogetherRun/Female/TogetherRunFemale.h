// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NPC/TogetherRun/TogetherRunBase.h"
#include "TogetherRunFemale.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ATogetherRunFemale : public ATogetherRunBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float TickTime) override;

};
