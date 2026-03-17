#include "AI/Enemy/Boss/Task/DanceWalkingTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "AIController.h"

EBTNodeResult::Type UDanceWalkingTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    //UE_LOG(LogTemp, Error, TEXT("DanceWalk"));

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (!ControllingPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

    UBossAnimInstance* AnimInst = Cast<UBossAnimInstance>(ControllingPawn->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance());

    if (AnimInst)
    {
        AnimInst->SetbBossDanceAttack(true);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
