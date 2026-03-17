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
	void SetbBossNearingAttack(bool value) { bBossNearingAttack = value; }

	bool GetbBossDanceAttack() { return bBossDanceAttack; }
	bool GetbFindPlayer() { return bFindPlayer; }
	bool GetbBossDie() { return bBossDie; }
	bool GetbBossNearingAttack() { return bBossNearingAttack; }

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void EndBossCloud();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void EndBossDie();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartBossCloud();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartNearingAttack();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void EndNearingAttack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFindPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChargeMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBossDanceAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBossDie;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bBossNearingAttack;

private:
	APawn* OwningPawn;
	AAIController* AICon;
	UBlackboardComponent* BB;

};
