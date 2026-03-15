// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BurnActor/BurnActor.h"
#include "BurnActor_Meteor.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class USceneComponent;
class APawn;
class UParticleSystem;
class ABossEnemy;
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

	void SetIsBurn() override;
	void SetBossActor(ABossEnemy* value);

	// 프로젝트일 무브먼트 컴포넌트 선언
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	UProjectileMovementComponent* ProjectileMovement;

	// 자식에서 추가할 콜리전
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* MeteorCollision;

	UPROPERTY(BlueprintReadWrite, Category = "Meteor")
	float MeteorSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Meteor")
	int32 MeteorDamage;

	UPROPERTY(BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ExplosionEffect;

	FTimerHandle LaunchTimerHandle;
	FTimerHandle LocationLogTimerHandle;

	// 발사 함수
	UFUNCTION()
	void LaunchTowards();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	USceneComponent* Root;
	APawn* PlayerPawn;
	FVector PlayerLocation;
	ABossEnemy* BossActor;
};
