#include "AI/Enemy/Boss/Task/DanceWalkingTaskNode.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

EBTNodeResult::Type UDanceWalkingTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();

    if (!ControllingPawn || !MontageToPlay) return EBTNodeResult::Failed;

    

    UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();

    UAnimInstance* AnimInst = ControllingPawn->FindComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

    if (AnimInst->Montage_IsPlaying(MontageToPlay)) return EBTNodeResult::Succeeded;

    if (AnimInst)
    {
        AnimInst->Montage_Play(MontageToPlay);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
