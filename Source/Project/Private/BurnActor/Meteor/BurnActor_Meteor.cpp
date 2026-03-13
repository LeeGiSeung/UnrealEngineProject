// Fill out your copyright notice in the Description page of Project Settings.

#include "BurnActor/Meteor/BurnActor_Meteor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

ABurnActor_Meteor::ABurnActor_Meteor()
{
    PrimaryActorTick.bCanEverTick = true;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

    // 초기 설정
    ProjectileMovement->UpdatedComponent = RootComponent; // 업데이트할 컴포넌트 지정
    ProjectileMovement->InitialSpeed = 0.f;               // 처음엔 멈춰있음
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;   // 날아가는 방향으로 액터 회전
    ProjectileMovement->ProjectileGravityScale = 0.0f;     // 중력 0
    ProjectileMovement->bIsHomingProjectile = true;
    ProjectileMovement->HomingAccelerationMagnitude = 10000.f; // 유도 가속도
}

void ABurnActor_Meteor::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    if (PC)
    {
        PlayerPawn = PC->GetPawn();

        if (PlayerPawn)
        {
            Root = PlayerPawn->GetRootComponent();
            ProjectileMovement->HomingTargetComponent = Root;
        }
    }

    GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, this, &ABurnActor_Meteor::LaunchTowards, 3.0f, false);
}

void ABurnActor_Meteor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABurnActor_Meteor::LaunchTowards()
{

    // 속도 설정 및 발사
    float LaunchSpeed = 2000.0f;
    ProjectileMovement->Velocity = GetActorForwardVector() * LaunchSpeed;

    // 멈춰있던 상태 업데이트 활성화
    ProjectileMovement->UpdateComponentVelocity();
}