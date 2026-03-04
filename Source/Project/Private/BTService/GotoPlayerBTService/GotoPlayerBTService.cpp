// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService/GotoPlayerBTService/GotoPlayerBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTService.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"



UGotoPlayerBTService::UGotoPlayerBTService()
{
	// 이 설정이 있어야 주기적으로 틱이 실행됩니다.
	bNotifyTick = true;
	// OnBecomeRelevant를 사용하려면 이 설정도 필요합니다.
	bNotifyBecomeRelevant = true;
}

void UGotoPlayerBTService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	CachedPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent()) {
		BBoard = BB;
		BBoard->SetValueAsObject(TargetActorKey.SelectedKeyName, CachedPlayer.Get()); //PlayerPawn 찾은거 넘겨줌
	}

	if (BBoard) {
		TargetActor = Cast<AActor>(BBoard->GetValueAsObject(TargetActorKey.SelectedKeyName));
        BossActor = OwnerComp.GetOwner();
        //UE_LOG(LogTemp, Error, TEXT("Boss First Location : %s"), *BossActor->GetActorLocation().ToString());
        //UE_LOG(LogTemp, Error, TEXT("Player First Location : %s"), *TargetActor->GetActorLocation().ToString());
	}
}

void UGotoPlayerBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    if (!BossActor || !TargetActor) return;

	FVector PlayerLocation = TargetActor->GetActorLocation();
	FVector BossLocation = BossActor->GetActorLocation();

}
