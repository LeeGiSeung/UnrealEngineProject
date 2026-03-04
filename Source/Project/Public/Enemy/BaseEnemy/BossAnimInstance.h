// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAnimInstance.h"
#include "BossAnimInstance.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFindPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bChargeMana;

};
