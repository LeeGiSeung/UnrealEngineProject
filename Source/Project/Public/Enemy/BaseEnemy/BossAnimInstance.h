// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "BossAnimInstance.generated.h"

class AAIController;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class PROJECT_API UBossAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:
	
	void SetbFindPlayer(bool value) { bFindPlayer = value; }
	void SetbChargeMana(bool value) { bChargeMana = value; }
	void SetbBossDanceAttack(bool value) { bBossDanceAttack = value; }
	void SetbBossDie(bool value) { bBossDie = value; }

	bool GetbBossDanceAttack() { return bBossDanceAttack; }
	bool GetbFindPlayer() { return bFindPlayer; }
	bool GetbBossDie() { return bBossDie; }

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void EndBossCloud();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void EndBossDie();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartBossCloud();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFindPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChargeMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBossDanceAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBossDie;

private:
	APawn* OwningPawn;
	AAIController* AICon;
	UBlackboardComponent* BB;

};
