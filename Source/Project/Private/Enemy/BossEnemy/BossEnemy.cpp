// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy/BossEnemy.h"
#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"
#include "ElectricalDevice/Electrical_BossArm/Electrical_BossArm.h"

ABossEnemy::ABossEnemy()
{
    EnemyHP = 1;
}

void ABossEnemy::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;
    
    AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());

    AICon = Cast<AAIController>(GetController());

    BBComp = AICon->GetBlackboardComponent();

}

void ABossEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CurSpawnSocketTime += DeltaTime;

    TurnToPlayer(DeltaTime);

    if (CurSpawnSocketTime > 10.f) {
        DecreaseHP(1);
    }

    if (CurSpawnSocketTime > BaseSpawnSocketTime && CheckSpawnBossArm() == 0 && BBComp->GetValueAsBool(TEXT("bIsDead")) == false) {

        if (!ActorBPToSpawn) return;

        int BossArmNumber = FMath::RandRange(0, 4);

        switch (BossArmNumber)
        {
        case 0:
            SpawnBossArm(RightArmSocket);
            break;
        case 1:
            SpawnBossArm(LeftArmSocket);
            break;
        case 2:
            SpawnBossArm(RightCalfSocket);
            break;
        case 3:
            SpawnBossArm(LeftCalfSocket);
            break;
        default:
            break;
        }

        CurSpawnSocketTime = 0.f;
    }

}

void ABossEnemy::TurnToPlayer(float DeltaTime)
{
    // 1. 방향 벡터를 구하고 Z축(높낮이) 차이 무시 (보스가 하늘이나 땅을 보며 기울어지는 것 방지)
    FVector Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
    Direction.Z = 0.0f;

    // 2. 플레이어를 바라보기 위한 목표 회전값 계산
    FRotator TargetRotation = Direction.Rotation();

    // 3. 보스의 현재 회전값
    FRotator CurrentRotation = GetActorRotation();

    // 4. RInterpTo를 사용한 부드러운 보간 (InterpSpeed 값이 클수록 빨리 돕니다)
    float InterpSpeed = 500.0f;
    FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, InterpSpeed);

    // 5. 보스 액터에 부드러워진 회전값 적용
    SetActorRotation(SmoothRotation);
}

void ABossEnemy::EnemyDie()
{
    Super::EnemyDie();

    // 1. 배열을 순회하며 모든 팔 액터를 제거
    for (AActor* Arm : SpawnedBossArms)
    {
        if (IsValid(Arm)) // 아직 유효한(삭제되지 않은) 상태인지 확인
        {
            Arm->Destroy();
        }
    }

    // 2. 배열 비우기 (메모리 정리)
    SpawnedBossArms.Empty();

    if (AICon)
    {

        if (BBComp)
        {
            UE_LOG(LogTemp, Error, TEXT("Boss Actor Die"));
            // 3. 값 변경 (예: IsDead 변수를 true로)
            BBComp->SetValueAsBool(TEXT("bIsDead"), true);
        }
    }

    //Destroy();
}

void ABossEnemy::OnSpawnBossArm()
{
    bSpawnBossArm++;
}

void ABossEnemy::OffSpawnBossArm()
{
    bSpawnBossArm--;

    if (bSpawnBossArm <= 0) bSpawnBossArm = 0;
        
}

int ABossEnemy::CheckSpawnBossArm()
{
    return bSpawnBossArm;
}

void ABossEnemy::SpawnBossArm(FName _SocketName)
{
    SocketName = _SocketName;

    SocketLocation = GetMesh()->GetSocketLocation(SocketName);
    SocketRotation = GetMesh()->GetSocketRotation(SocketName);

    SpawnParams.Owner = this;

    SpawnedSocketActor = GetWorld()->SpawnActor<AActor>(ActorBPToSpawn, SocketLocation, SocketRotation, SpawnParams);

    if (SpawnedSocketActor)
    {
        FAttachmentTransformRules AttachRules(
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::SnapToTarget,
            EAttachmentRule::KeepWorld,
            true
        );
        SpawnedSocketActor->AttachToComponent(GetMesh(), AttachRules, SocketName);
        AElectrical_BossArm* BossArm = Cast<AElectrical_BossArm>(SpawnedSocketActor);
        BossArm->SetBossPointer(this);
        SpawnedBossArms.Add(BossArm);
        OnSpawnBossArm();
    }
}
