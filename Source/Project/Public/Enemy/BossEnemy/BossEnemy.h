// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy/BaseEnemy.h"
#include "BossEnemy.generated.h"

class UBossAnimInstance;
class UBlackboardComponent;
class AAIController;
/**
 * 
 */
UCLASS()
class PROJECT_API ABossEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	ABossEnemy();
	virtual void Tick(float DeltaTime) override;

	void TurnToPlayer(float DeltaTime);

	virtual void EnemyDie() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
	TSubclassOf<AActor> ActorBPToSpawn;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* DancingWalkMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* MagicPowerMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* MagicCloudMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "BossArm")
	float BaseSpawnSocketTime = 0.f;

	TArray<AActor*> SpawnedBossArms;

	void OnSpawnBossArm();
	void OffSpawnBossArm();
	int CheckSpawnBossArm();

private:

	AAIController* AICon;
	UBlackboardComponent* BBComp;

	FName RightArmSocket = FName("RightArmSocket");
	FName LeftArmSocket = FName("LeftArmSocket");
	FName RightCalfSocket = FName("RightCalfSocket");
	FName LeftCalfSocket = FName("LeftCalfSocket");

	FName SocketName;

	AActor* SpawnedSocketActor;
	FActorSpawnParameters SpawnParams;

	FVector SocketLocation;
	FVector SocketScale;
	FRotator SocketRotation;

	UBossAnimInstance* AnimInst;

	
	float CurSpawnSocketTime = 0.f;

	void SpawnBossArm(FName _SocketName);

	int bSpawnBossArm = 0;

};
