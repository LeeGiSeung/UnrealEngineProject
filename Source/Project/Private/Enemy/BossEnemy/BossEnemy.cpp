// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BossEnemy/BossEnemy.h"
#include "Enemy/BaseEnemy/BossAnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "ElectricalDevice/Electrical_BossArm/Electrical_BossArm.h"

ABossEnemy::ABossEnemy()
{
    EnemyHP = 2;
}

void ABossEnemy::BeginPlay()
{
    Super::BeginPlay();
    PrimaryActorTick.bCanEverTick = true;
    
    if (ActorBPToSpawn)
    {
        // 2. 소켓 이름 지정 (에디터에 만든 소켓 이름과 동일해야 함)
        SocketName = RightArmSocket;

        // 3. 소켓의 위치와 회전값 가져오기
        SocketLocation = GetMesh()->GetSocketLocation(SocketName);
        SocketRotation = GetMesh()->GetSocketRotation(SocketName);
        
        // 4. 스폰 파라미터 설정 (누가 소환했는지 등)
        SpawnParams.Owner = this;

        // 5. 드디어 월드에 스폰!
        SpawnedSocketActor = GetWorld()->SpawnActor<AActor>(ActorBPToSpawn, SocketLocation, SocketRotation, SpawnParams);

        // (선택) 만약 스폰한 액터를 소켓에 찰싹 붙여서 같이 움직이게 하고 싶다면 아래 코드 추가

        if (SpawnedSocketActor)
        {
            FAttachmentTransformRules AttachRules(
                EAttachmentRule::SnapToTarget,
                EAttachmentRule::SnapToTarget,
                EAttachmentRule::KeepWorld,
                true
            );

            //SpawnedSocketActor->SetActorScale3D(SocketScale);
            SpawnedSocketActor->AttachToComponent(GetMesh(), AttachRules, SocketName);
            AElectrical_BossArm* BossArm = Cast<AElectrical_BossArm>(SpawnedSocketActor);
            BossArm->SetBossPointer(this);
        }

        //// 2. 소켓 이름 지정 (에디터에 만든 소켓 이름과 동일해야 함)
        //SocketName = LeftArmSocket;

        //// 3. 소켓의 위치와 회전값 가져오기
        //SocketLocation = GetMesh()->GetSocketLocation(SocketName);
        //SocketRotation = GetMesh()->GetSocketRotation(SocketName);
        //
        //// 4. 스폰 파라미터 설정 (누가 소환했는지 등)
        //SpawnParams.Owner = this;

        //// 5. 드디어 월드에 스폰!
        //SpawnedSocketActor = GetWorld()->SpawnActor<AActor>(ActorBPToSpawn, SocketLocation, SocketRotation, SpawnParams);

        //// (선택) 만약 스폰한 액터를 소켓에 찰싹 붙여서 같이 움직이게 하고 싶다면 아래 코드 추가

        //if (SpawnedSocketActor)
        //{
        //    FAttachmentTransformRules AttachRules(
        //        EAttachmentRule::SnapToTarget,
        //        EAttachmentRule::SnapToTarget,
        //        EAttachmentRule::KeepWorld,
        //        true
        //    );

        //    //SpawnedSocketActor->SetActorScale3D(SocketScale);
        //    SpawnedSocketActor->AttachToComponent(GetMesh(), AttachRules, SocketName);
        //}

    }  
    
    AnimInst = Cast<UBossAnimInstance>(GetMesh()->GetAnimInstance());

}

void ABossEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //t += DeltaTime;

    //if (t > 5.f) {
    //    AnimInst->SetbFindPlayer(true);
    //}
    
}
