// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemy/Boss/Task/NearingAttack/NearingAttackTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "AIController.h"

EBTNodeResult::Type UNearingAttackTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (!ControllingPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

    UBossAnimInstance* AnimInst = Cast<UBossAnimInstance>(ControllingPawn->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance());

    if (AnimInst)
    {
        AnimInst->SetbBossNearingAttack(true);
        return EBTNodeResult::Succeeded;
    }

	return EBTNodeResult::Type();
}
