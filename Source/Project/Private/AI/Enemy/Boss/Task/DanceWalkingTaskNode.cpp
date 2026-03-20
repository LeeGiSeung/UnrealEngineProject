#include "AI/Enemy/Boss/Task/DanceWalkingTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

EBTNodeResult::Type UDanceWalkingTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

    CachedOwnerComp = &OwnerComp;

    if (!ControllingPawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

    UBossAnimInstance* AnimInst = Cast<UBossAnimInstance>(ControllingPawn->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance());

    if (AnimInst)
    {
        GetWorld()->GetTimerManager().SetTimer(MeteorCoolTime, this, &UDanceWalkingTaskNode::ResetMetorCoolTime, 3.f, false);
        AnimInst->SetbBossDanceAttack(true);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

void UDanceWalkingTaskNode::ResetMetorCoolTime()
{
    UBlackboardComponent* BBComp = CachedOwnerComp->GetBlackboardComponent();

    BBComp->SetValueAsBool(TEXT("bMeteorCoolTime"), false);
}
