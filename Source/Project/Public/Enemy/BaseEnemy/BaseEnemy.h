// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BaseEnemy.generated.h"

class AAIController;
class UBlackboardComponent;

UCLASS()
class PROJECT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AAIController* AIController;
	UBlackboardComponent* BBoard;
	APawn* PlayerPawn;

	virtual void IncreaseHP(int value);
	virtual void DecreaseHP(int value);

	virtual void EnemyDie();

protected:
	uint32 EnemyHP;

};
