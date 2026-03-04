#include "AI/Enemy/Boss/Task/Boss_MoveTo.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UBoss_MoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (!ControllingPawn || !MontageRunPlay) return EBTNodeResult::Failed;

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

    UAnimInstance* AnimInst = ControllingPawn->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
    if (AnimInst)
    {
        AnimInst->Montage_Play(MontageRunPlay);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}