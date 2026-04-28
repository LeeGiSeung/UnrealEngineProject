// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/TogetherRun/TogetherRunBase.h"

#include "ProjectCharacter.h"
#include "NPC/TogetherRun/AnimInstance/TogetherRunAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATogetherRunBase::ATogetherRunBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 혹은 ECR_Block
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

}

void ATogetherRunBase::BeginPlay()
{
	Super::BeginPlay();
	
	BasicSetting();

}

void ATogetherRunBase::BasicSetting()
{

	TogetherRunAnimInstance = Cast<UTogetherRunAnimInstance>(GetMesh()->GetAnimInstance());
	TogetherRunAnimInstance->SetNPCReference(this); //NPC 주소 참조시켜줌
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 1. 물리와 감지 모두 켭니다.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 2. NPC의 정체성(Object Type)을 확실하게 'Pawn'으로 강제합니다.
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);

	// 3. 중요: NPC 입장에서 다가오는 Player의 구체(WorldDynamic)를 허용(Overlap)해 줍니다.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	// 4. 오버랩 이벤트를 켭니다.
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);

	// 2. 그 중 Pawn(플레이어의 감지 구체)에 대해서만 Overlap으로 설정
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 3. 물리 상태 확인
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ATogetherRunBase::SetFrontActorReference(AProjectCharacter* value)
{
	Player = value;

	GetCapsuleComponent()->IgnoreActorWhenMoving(value, true);
}

// Called every frame
void ATogetherRunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return; //PlayerCharacter가 없으면 검사 조차안됨

	SetTogetherActorTurn();

}

void ATogetherRunBase::SetTogetherActorTurn()
{
	FVector Direction = Player->GetActorForwardVector();

	AddMovementInput(Direction, 1.0f);
}

// Called to bind functionality to input
void ATogetherRunBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATogetherRunBase::SetTogetherActorSpeed(AActor* FrontActor, float value, FVector HandLocation)
{

}

AActor* ATogetherRunBase::GetTogetherBaseLeaderObject()
{
	return Player;
}

void ATogetherRunBase::SetChainIndex(int value)
{
	chainIndex = value;
}

int ATogetherRunBase::GetChainIndex()
{
	return chainIndex;
}


