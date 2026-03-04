// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Enemy/EnemyAIController.h"
#include "BossAIController.generated.h"

class UBehaviorTree;
class UBlackboardComponent;

/**
 * 
 */
UCLASS()
class PROJECT_API ABossAIController : public AEnemyAIController
{
	GENERATED_BODY()
	
public:
	void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "BlackBoard")
	UBehaviorTree* BTAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BlackBoard")
	UBlackboardComponent* BBComponent;

};
