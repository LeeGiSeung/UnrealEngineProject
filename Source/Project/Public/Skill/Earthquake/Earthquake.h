// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase/SkillBase.h"
#include "Earthquake.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AEarthquake : public ASkillBase
{
	GENERATED_BODY()

public:
	AEarthquake();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UPROPERTY()
	FTimerHandle DeleteEarthquake;

	void OnEarthquakeTimerFinished();
};
