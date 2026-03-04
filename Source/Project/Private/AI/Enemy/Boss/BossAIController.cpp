// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Enemy/Boss/BossAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

void ABossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BTAsset) {
		RunBehaviorTree(BTAsset);

		BBComponent = GetBlackboardComponent();

		if (BBComponent) {
			BBComponent->SetValueAsObject(FName("TargetActor"), Cast<AActor>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)));
		}

	}
}
