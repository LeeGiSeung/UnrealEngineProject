// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService/PlayerDistanceBTService/PlayerDistanceBTService.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BTService.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "Kismet/GameplayStatics.h"

UPlayerDistanceBTService::UPlayerDistanceBTService()
{
    // 이 설정이 있어야 주기적으로 틱이 실행됩니다.
    bNotifyTick = true;
    // OnBecomeRelevant를 사용하려면 이 설정도 필요합니다.
    bNotifyBecomeRelevant = true;
}

void UPlayerDistanceBTService::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // 여기서 딱 한 번만 플레이어를 찾아서 캐싱해둡니다.
    CachedPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    AIController = OwnerComp.GetAIOwner();

    if (AIController) Boss = AIController->GetPawn();

    if (ACharacter* BossCharacter = Cast<ACharacter>(Boss)) {
        USkeletalMeshComponent* Mesh = BossCharacter->GetMesh();
        AnimInst = Cast<UBossAnimInstance>(Mesh->GetAnimInstance());
    }

}

void UPlayerDistanceBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    BBoard = OwnerComp.GetBlackboardComponent();
    
    TargetActor = Cast<AActor>(BBoard->GetValueAsObject(FName("TargetActor")));

    if (!Boss) {
        UE_LOG(LogTemp, Error, TEXT("TargetActorNone"));
        return;
    }

    if (!TargetActor) {
        UE_LOG(LogTemp, Error, TEXT("TargetActorNone"));
        return;
    }

    if (!BBoard) {
        UE_LOG(LogTemp, Error, TEXT("TargetActorNone"));
        return;
    }

    float Distance = Boss->GetDistanceTo(TargetActor);
    

    FVector Start = Boss->GetActorLocation();
    FVector Target = TargetActor->GetActorLocation();

    // Z축(높이)은 유지하고 평면상에서만 회전하도록 설정 (필요시)
    Target.Z = Start.Z;

    FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);

    //UE_LOG(LogTemp, Error, TEXT("Look at : %s"), *LookAtRotation.ToString());

    // 2. 부드러운 회전 적용 (RInterpTo 사용)
    FRotator CurrentRotation = Boss->GetActorRotation();
    FRotator SmoothedRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, DeltaSeconds, 5.0f); // 5.0f는 회전 속도

    Boss->SetActorRotation(SmoothedRotation);
    
    //UE_LOG(LogTemp, Error, TEXT("Distance : %s"), *SmoothedRotation.ToString());

    // 거리 조건 (600 이하)
    bool bInAttackRange = (Distance <= 1000.f);

    BBoard->SetValueAsFloat(TEXT("Distance"), Distance);

    FString BoolString = bInAttackRange ? TEXT("true") : TEXT("false");

    //UE_LOG(LogTemp, Error, TEXT("Distance : %f"), Distance);

    //if (Distance < 2500 && !bInAttackRange && !AnimInst->GetbFindPlayer()) { //2000이하, 공격 거리에 없고, 아직 Player를 찾지 못했을때
    //    AnimInst->SetbFindPlayer(true);
    //}
    //else if (bInAttackRange) {
    //    AnimInst->SetbFindPlayer(false);
    //    //BBoard->SetValueAsBool(TEXT("CanAttack"), bInAttackRange);
    //    //또 다른 공격 방법 하나 추가하기
    //    UE_LOG(LogTemp, Error, TEXT("<<< 700"));
    //}
    //else if (Distance > 2500 && Distance < 6000.f && !bInAttackRange) {
    //    AnimInst->SetbFindPlayer(false);
    //    UE_LOG(LogTemp, Error, TEXT("Meteor!!!!!!!!!!!!!!!!!!!"));
    //    BBoard->SetValueAsBool(TEXT("CanAttack"), true);
    //}
    //else if(Distance > 6000.f){
    //    AnimInst->SetbFindPlayer(false);
    //}

    if (Distance <= 1000.f && !BBoard->GetValueAsBool(TEXT("bNearingAttackCoolTime"))) // 근접 공격
    {
        AnimInst->SetbFindPlayer(false);
        BBoard->SetValueAsBool(TEXT("bNearingAttack"), true);
        BBoard->SetValueAsBool(TEXT("bNearingAttackCoolTime"), true);
        //UE_LOG(LogTemp, Warning, TEXT("Close Attack"));
    }
    else if (Distance >= 3000.f && !BBoard->GetValueAsBool(TEXT("bMeteorCoolTime"))) // 메테오 공격
    {
        AnimInst->SetbFindPlayer(false);
        BBoard->SetValueAsBool(TEXT("CanAttack"), true);
        BBoard->SetValueAsBool(TEXT("bMeteorCoolTime"), true);
        //UE_LOG(LogTemp, Warning, TEXT("Meteor Attack"));
    }
    else // 700 ~ 2500 → 추적
    {
        AnimInst->SetbFindPlayer(true);
        //UE_LOG(LogTemp, Warning, TEXT("Chasing Player"));
    }

}
