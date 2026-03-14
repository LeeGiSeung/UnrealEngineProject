// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BurnActor/BurnActor.h"
#include "BurnActor_Meteor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class USceneComponent;
class APawn;

/**
 * 
 */
UCLASS()
class PROJECT_API ABurnActor_Meteor : public ABurnActor
{
	GENERATED_BODY()
public:
	ABurnActor_Meteor();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// 프로젝트일 무브먼트 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	// 자식에서 추가할 콜리전
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* MeteorCollision;

	FTimerHandle LaunchTimerHandle;
	FTimerHandle LocationLogTimerHandle;

	// 발사 함수
	void LaunchTowards();

	USceneComponent* Root;
	APawn* PlayerPawn;
	FVector PlayerLocation;
};
