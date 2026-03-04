// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService/BaseBTService.h"
#include "PlayerDistanceBTService.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UPlayerDistanceBTService : public UBaseBTService
{
	GENERATED_BODY()
	
public:
	UPlayerDistanceBTService();

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 이 서비스가 사용할 블랙보드 키를 선택하는 변수입니다.
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

private:
	// 서비스 내부에서만 기억할 플레이어 포인터
	TWeakObjectPtr<AActor> CachedPlayer;
	UBlackboardComponent* BBoard;

	AActor* TargetActor;
	AActor* Boss;
	AAIController* AIController;
};
