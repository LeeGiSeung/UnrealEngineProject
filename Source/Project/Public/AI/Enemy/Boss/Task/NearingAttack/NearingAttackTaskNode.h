// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "NearingAttackTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UNearingAttackTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FTimerHandle NearingAttackCoolTime;

	void ResetNearingAttackCoolTime();

	UBehaviorTreeComponent* CachedOwnerComp;

};
