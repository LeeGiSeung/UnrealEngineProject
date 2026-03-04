// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService/BaseBTService.h"
#include "GotoPlayerBTService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UGotoPlayerBTService : public UBaseBTService
{
	GENERATED_BODY()
public:
	UGotoPlayerBTService();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 이 서비스가 사용할 블랙보드 키를 선택하는 변수입니다.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

private:
	AActor* TargetActor;
	AActor* BossActor;

	TWeakObjectPtr<AActor> CachedPlayer;
	UBlackboardComponent* BBoard;
};
