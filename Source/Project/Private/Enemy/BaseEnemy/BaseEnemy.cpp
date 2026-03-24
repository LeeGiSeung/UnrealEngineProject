// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy/BaseEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "BrainComponent.h"
#include "Minimap/MinimapWorldSystem.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

    // 1. AI 컨트롤러와 블랙보드 가져오기
    if (AAIController* AIC = Cast<AAIController>(GetController()))
    {
        AIController = AIC;
        if (UBlackboardComponent* BB = AIC->GetBlackboardComponent())
        {
            // 2. 월드에서 0번 플레이어 찾기
            BBoard = BB;
            APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

            if (Player)
            {
                PlayerPawn = Player;
                // 3. 블랙보드의 'TargetActor' 키에 플레이어 세팅 (키 이름은 블랙보드와 일치해야 함)
                BB->SetValueAsObject(TEXT("TargetActor"), PlayerPawn);
            }
        }
    }
	
    if (UWorld* World = GetWorld())
    {
        MinimapWorld = World->GetSubsystem<UMinimapWorldSystem>();
    }

}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::IncreaseHP(int value)
{
    EnemyHP += value;
}

void ABaseEnemy::DecreaseHP(int value)
{
    if (EnemyHP <= 0) return;
    EnemyHP -= value;

    if (EnemyHP <= 0) {
        EnemyDie();
        return;
    }
    
}

void ABaseEnemy::EnemyDie()
{
    //// 1. 비헤이비어 트리 중지 (AI가 더 이상 동작하지 않도록)
    //if (AAIController* AICon = Cast<AAIController>(GetController()))
    //{
    //    AICon->BrainComponent->StopLogic("Boss is Dead");
    //}

    // 2. 콜리전 끄기 (시체가 바닥을 뚫거나 플레이어를 막지 않도록)
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    //#######################################
    //자식 객체에서 항상 Destory() 꼭 해줘야함
    //#######################################

}

