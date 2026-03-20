// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DanceWalkingTaskNode.generated.h"

class UBehaviorTreeComponent;

/**
 * 
 */
UCLASS()
class PROJECT_API UDanceWalkingTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FTimerHandle MeteorCoolTime;

	void ResetMetorCoolTime();

	UBehaviorTreeComponent* CachedOwnerComp;

};
