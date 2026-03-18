// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy/BossEnemy.h"
#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"
#include "ElectricalDevice/Electrical_BossArm/Electrical_BossArm.h"
#include "BurnActor/Meteor/BurnActor_Meteor.h"
#include "Skill/Earthquake/Earthquake.h"
#include "Components/CapsuleComponent.h"

ABossEnemy::ABossEnemy()
{
    EnemyHP = 3;
}

void ABossEnemy::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;
    
    AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());

    AICon = Cast<AAIController>(GetController());

    BBComp = AICon->GetBlackboardComponent();

    //GetWorld()->GetTimerManager().SetTimer(MeteorSkillTimerHandle, this, &ABossEnemy::SpawnMeteor, 1.0f, true);
    //GetWorld()->GetTimerManager().SetTimer(SpawnBossArmTimerHandle, this, &ABossEnemy::SpawnBossArm, BaseSpawnSocketTime, true);

}

void ABossEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TurnToPlayer(DeltaTime);

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

void ABossEnemy::SpawnBossArm()
{
    if (!ActorBPToSpawn || SpawnedBossArms.Num() != 0 || BBComp->GetValueAsBool(TEXT("bIsDead")) != false) return;

    int BossArmNumber = FMath::RandRange(0, 4);

    switch (BossArmNumber)
    {
    case 0:
        SocketName = RightArmSocket;
        break;
    case 1:
        SocketName = LeftArmSocket;
        break;
    case 2:
        SocketName = RightCalfSocket;
        break;
    case 3:
        SocketName = LeftCalfSocket;
        break;
    default:
        break;
    }

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

void ABossEnemy::SpawnMeteor()
{
    if (!MeteorBPToSpawn) return;
    if (SpawnedMeteor.Num() != 0) return;

    SpawnEachMeteor(MeteorSocket_0);
    SpawnEachMeteor(MeteorSocket_1);
    SpawnEachMeteor(MeteorSocket_2);

}

void ABossEnemy::NearingAttack()
{
    if (!EarthquakeBPToSpawn) return;

    GetWorldTimerManager().SetTimer(SpawnFirstEarthquake, this, &ABossEnemy::SpawnFirst, 1.5f, false);
    GetWorldTimerManager().SetTimer(SpawnSecondEarthquake, this, &ABossEnemy::SpawnSecond, 2.5f, false);
    GetWorldTimerManager().SetTimer(SpawnThrityEarthquake, this, &ABossEnemy::SpawnThrity, 3.5f, false);

}

void ABossEnemy::SpawnFirst()
{
    SpawnEarthquake(25, 800);
}

void ABossEnemy::SpawnSecond()
{
    SpawnEarthquake(40, 1500);
}

void ABossEnemy::SpawnThrity()
{
    SpawnEarthquake(60, 2100);
}

void ABossEnemy::SpawnEarthquake(int32 BlockCount, float _Radius) {
    // 배치 설정 변수들 (필요에 따라 함수의 매개변수나 UPROPERTY로 빼서 사용하세요)
    int32 NumberOfBlocks = BlockCount;       // 스폰할 블럭 개수
    float Radius = _Radius;          // 보스로부터의 거리 (반지름)
    float ArcAngle = 360.0f;        // 블럭이 배치될 총 각도 (180도 = 반원)

    FVector CenterLocation = GetActorLocation(); // 보스의 현재 위치
    FRotator CenterRotation = GetActorRotation(); // 보스의 현재 회전값

    // 시작 각도 계산 (보스 정면을 기준으로 좌우 대칭이 되도록 절반을 뺌)
    float StartAngle = -ArcAngle / 2.0f;

    // 블럭 간의 각도 간격 (블럭이 1개일 때의 0 나누기 방지)
    float AngleStep = ArcAngle / FMath::Max(1, (NumberOfBlocks - 1));

    for (int32 i = 0; i < NumberOfBlocks; ++i)
    {
        // 현재 블럭의 각도 계산
        float CurrentAngle = StartAngle + (AngleStep * i);

        // 보스의 정면 벡터를 Z축 기준으로 CurrentAngle만큼 회전
        FRotator RotationOffset(0.f, CurrentAngle, 0.f);
        FVector Direction = RotationOffset.RotateVector(CenterRotation.Vector());

        // 최종 스폰 위치: 보스 위치 + (방향 * 반지름)
        FVector SpawnLocation = CenterLocation + (Direction * Radius);

        // 블럭의 회전 설정 
        // 1. 보스 바깥쪽을 바라보게 하려면: Direction.Rotation()
        // 2. 이미지처럼 블럭의 특정 면이 보스 쪽을 향하게 하려면 추가 회전(Offset) 적용
        FRotator SpawnRotation = Direction.Rotation();

        float x = FMath::RandRange(1, 180);
        float y = FMath::RandRange(1, 180);
        float z = FMath::RandRange(1, 180);

        SpawnRotation.Yaw = x;
        SpawnRotation.Roll = y;
        SpawnRotation.Pitch = z;

        // 기존의 Z값을 바닥 높이로 덮어씌웁니다.
        float FloorZ = GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

        FloorZ -= 50.f;

        // XY는 앞서 계산한 위치를 쓰고, Z만 FloorZ로 설정
        FVector FinalSpawnLocation = FVector(SpawnLocation.X, SpawnLocation.Y, FloorZ);

        // (선택) 지형에 맞게 높이(Z)를 조정하려면 여기서 Line Trace를 사용하여 Z값을 덮어씌웁니다.

        FActorSpawnParameters LocaSpawnParams;
        LocaSpawnParams.Owner = this;
        LocaSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        GetWorld()->SpawnActor<AActor>(EarthquakeBPToSpawn, FinalSpawnLocation, SpawnRotation, LocaSpawnParams);
    }
}

void ABossEnemy::SpawnEachMeteor(FName _SocketName)
{

    FVector LocalSocketLocation = GetMesh()->GetSocketLocation(_SocketName);
    FRotator LocalSocketRotation = GetMesh()->GetSocketRotation(_SocketName);

    FActorSpawnParameters LocalSpawnPar;
    LocalSpawnPar.Owner = this;

    // 지역 변수로 스폰된 액터 받기
    AActor* LocalSpawnedActor = GetWorld()->SpawnActor<AActor>(MeteorBPToSpawn, LocalSocketLocation, LocalSocketRotation, LocalSpawnPar);
    Cast<ABurnActor_Meteor>(LocalSpawnedActor)->SetBossActor(this);

    if (LocalSpawnedActor)
    {
        SpawnedMeteor.Add(LocalSpawnedActor);
    }
}

void ABossEnemy::DestoryMeteor(AActor* Meteor)
{
    SpawnedMeteor.Remove(Meteor);
}
