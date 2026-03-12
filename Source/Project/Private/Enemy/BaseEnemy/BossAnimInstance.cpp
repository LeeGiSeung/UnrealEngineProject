// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBossAnimInstance::EndBossCloud()
{
    // 1. 이 애니메이션을 실행 중인 Pawn(보스)을 가져옵니다.
    OwningPawn = TryGetPawnOwner();
    if (OwningPawn)
    {
        // 2. Pawn의 컨트롤러를 가져와 AIController로 캐스팅합니다.
        AICon = Cast<AAIController>(OwningPawn->GetController());
        if (AICon)
        {
            // 3. AIController가 가진 블랙보드 컴포넌트에 접근합니다.
            BB = AICon->GetBlackboardComponent();
            if (BB)
            {
                // 블랙보드 키 값을 수정합니다.
                BB->SetValueAsBool(TEXT("CanAttack"), false);
                // 4. ABP 내부 변수도 여기서 같이 수정 가능합니다.
                SetbBossDanceAttack(false);
            }
        }
    }
}

void UBossAnimInstance::EndBossDie()
{
    OwningPawn = TryGetPawnOwner();
    if (OwningPawn)
    {
        AICon = Cast<AAIController>(OwningPawn->GetController());
        if (AICon)
        {
            BB = AICon->GetBlackboardComponent();
            if (BB)
            {
                // 블랙보드 키 값을 수정합니다.
                BB->SetValueAsBool(TEXT("bIsDead"), false);
                OwningPawn->Destroy(); //보스 삭제
            }
        }
    }
}
