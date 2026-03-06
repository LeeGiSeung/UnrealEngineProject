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
    EnemyHP = 2;
}

void ABossEnemy::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;
    
    AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());

}

void ABossEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CurSpawnSocketTime += DeltaTime;

    if (CurSpawnSocketTime > BaseSpawnSocketTime && CheckSpawnBossArm() == 0) {

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
        OnSpawnBossArm();
    }
}
