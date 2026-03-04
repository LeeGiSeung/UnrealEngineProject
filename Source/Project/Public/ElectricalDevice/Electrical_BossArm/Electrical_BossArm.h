// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElectricalDevice/ElectricalDevice.h"
#include "Electrical_BossArm.generated.h"

class ABossEnemy;

/**
 * 
 */
UCLASS()
class PROJECT_API AElectrical_BossArm : public AElectricalDevice
{
	GENERATED_BODY()

public:
	AElectrical_BossArm();

	virtual void Use_Function();

	void SetBossPointer(ABossEnemy* value);

private:
	ABossEnemy* BossActor;

};
