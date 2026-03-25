// Fill out your copyright notice in the Description page of Project Settings.

#include "BurnActor/Meteor/BurnActor_Meteor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "ProjectCharacter.h"
#include "Enemy/BossEnemy/BossEnemy.h"
#include "Drawing_Fire_Actor.h"

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
    ProjectileMovement->MaxSpeed = MeteorSpeed;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->bIsHomingProjectile = false; //유도탄 끔

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

            MeteorCollision->OnComponentBeginOverlap.AddDynamic(this, &ABurnActor_Meteor::BeginOverlap);
        }
    }

    GetWorld()->GetTimerManager().SetTimer(LaunchTimerHandle, this, &ABurnActor_Meteor::LaunchTowards, 5.0f, false);
}

void ABurnActor_Meteor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABurnActor_Meteor::SetIsBurn()
{
    Super::SetIsBurn();
    
    BossActor->DestoryMeteor(this);

}

void ABurnActor_Meteor::SetBossActor(ABossEnemy* value)
{
    BossActor = value;
}

void ABurnActor_Meteor::LaunchTowards()
{
    //UE_LOG(LogTemp, Error, TEXT("LaunchTowards"));

    ProjectileMovement->Activate(false);

    FVector LocalPlayerLocation = PlayerPawn->GetActorLocation();
    FVector LocalMeteorLocation = GetActorLocation();

    FVector DirectionVector = (LocalPlayerLocation - LocalMeteorLocation).GetSafeNormal();

    // 속도 설정 및 발사
    float LaunchSpeed = MeteorSpeed;
    ProjectileMovement->Velocity = DirectionVector * LaunchSpeed;

    // 멈춰있던 상태 업데이트 활성화
    ProjectileMovement->UpdateComponentVelocity();
}

void ABurnActor_Meteor::BeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

    if (Cast<ABurnActor_Meteor>(OtherActor)) return;

    if (OtherActor == GetOwner()) return;
    if (ADrawing_Fire_Actor* FireDrawing = Cast<ADrawing_Fire_Actor>(OtherActor)) return; 

    if (OtherActor == PlayerPawn)
    {
        AProjectCharacter* PlayerCharacter = Cast<AProjectCharacter>(PlayerPawn);
        if (PlayerCharacter)
        {
            PlayerCharacter->DecreasePlayerHP(MeteorDamage);
        }
    }

    UE_LOG(LogTemp, Error, TEXT("HIT: %s"), *OtherActor->GetName());

    if (ExplosionEffect)
    {
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), GetActorScale3D());
    }

    BossActor->DestoryMeteor(this);

    Destroy();

}