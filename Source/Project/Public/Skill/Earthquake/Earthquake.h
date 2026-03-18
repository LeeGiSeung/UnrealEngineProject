// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Skill/SkillBase/SkillBase.h"
#include "Earthquake.generated.h"

class UBoxComponent;

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
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	void OnEarthquakeTimerFinished();

	UFUNCTION()
	void OnBlockOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
};
