// Fill out your copyright notice in the Description page of Project Settings.

#include "BurnActor/Meteor/BurnActor_Meteor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"

ABurnActor_Meteor::ABurnActor_Meteor()
{
    PrimaryActorTick.bCanEverTick = true;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

    // 2. 부모의 RootComponent를 이동 대상으로 지정합니다.
    // 생성자 시점에서는 RootComponent가 비어있을 수 있으므로 
    // 나중에 BeginPlay에서 다시 한 번 확인해주는 것이 안전합니다.
    ProjectileMovement->UpdatedComponent = RootComponent;

    MeteorCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MeteorCollision"));

    // 2. [핵심] 부모가 설정한 RootComponent(Mesh) 아래에 부착합니다.
    MeteorCollision->SetupAttachment(RootComponent);
    MeteorCollision->InitSphereRadius(40.0f);
    MeteorCollision->SetCollisionProfileName(TEXT("Projectile"));

    // 발사체 설정
    ProjectileMovement->InitialSpeed = 0.f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->bIsHomingProjectile = true;
    ProjectileMovement->HomingAccelerationMagnitude = 10000.f;

    ProjectileMovement->bAutoActivate = false;
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

    ProjectileMovement->Activate(false);

    // 속도 설정 및 발사
    float LaunchSpeed = 2000.0f;
    ProjectileMovement->Velocity = GetActorForwardVector() * LaunchSpeed;

    // 멈춰있던 상태 업데이트 활성화
    ProjectileMovement->UpdateComponentVelocity();
}