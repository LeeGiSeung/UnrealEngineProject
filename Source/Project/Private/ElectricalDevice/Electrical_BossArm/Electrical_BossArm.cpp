// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricalDevice/Electrical_BossArm/Electrical_BossArm.h"
#include "Enemy/BossEnemy/BossEnemy.h"

AElectrical_BossArm::AElectrical_BossArm()
{
	
}

void AElectrical_BossArm::Use_Function() {
	Super::Use_Function();
	
	if (!BossActor) {
		UE_LOG(LogTemp, Error, TEXT("NO BOSS ACTOR POINTER"));
		return;
	}
	BossActor->DecreaseHP(1);
	BossActor->OffSpawnBossArm();

}

void AElectrical_BossArm::SetBossPointer(ABossEnemy* value)
{
	BossActor = value;
}
