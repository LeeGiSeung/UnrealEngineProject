


// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "GameFramework/Character.h"          
#include "Components/SkeletalMeshComponent.h" 
#include "Animation/AnimInstance.h"
#include "GameFramework/PlayerController.h"
#include "ProjectCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwningCharacter = Cast<AProjectCharacter>(TryGetPawnOwner());

	if (OwningCharacter) {
		MovementComponent = Cast<UCharacterMovementComponent>(OwningCharacter->GetMovementComponent());
	}
}

void UBaseAnimInstance::PlaySpecialAttackMontage(ACharacter* APlayer)
{
	IsSpecialAttack = true;
	USkeletalMeshComponent* MeshComp = APlayer->GetMesh();

	if (!MeshComp) {
		UE_LOG(LogTemp, Warning, TEXT("No Mesh"));
		return;
	}

	if (SpecialAttackMontage)
	{
		Montage_Play(SpecialAttackMontage);
	}

}

void UBaseAnimInstance::PlayCrouchBackMontage(ACharacter* APlayer)
{
	IsCrouchBack = true;
	USkeletalMeshComponent* MeshComp = APlayer->GetMesh();
	UE_LOG(LogTemp, Warning, TEXT("No Mesh"));
	if (!MeshComp) {
		UE_LOG(LogTemp, Warning, TEXT("No Mesh"));
		return;
	}

	if (CrouchBackMontage)
	{
		Montage_Play(CrouchBackMontage);
		//UE_LOG(LogTemp, Warning, TEXT("Special Attack Montage Played"));
	}
}

void UBaseAnimInstance::AnimNotify_EndSpecialAttack(UAnimNotify* Notify)
//AnimNotify 자동 호출 규칙
//애니메이션(몽타주/시퀀스)에 추가한 Notify 이름이 정확히 같아야 함
//AnimNotify_ + Notify name
{
	IsSpecialAttack = false;
	OnSpecialAttackFinished.Broadcast();
}

void UBaseAnimInstance::AnimNotify_EndCrouchBack(UAnimNotify* Notify)
//AnimNotify 자동 호출 규칙
//애니메이션(몽타주/시퀀스)에 추가한 Notify 이름이 정확히 같아야 함
//AnimNotify_ + Notify name
{
	IsCrouchBack = false;
	OnCrouchBackFinished.Broadcast();
}

void UBaseAnimInstance::EndClimbStand()
{
	//UE_LOG(LogTemp, Error, TEXT("SetClimbStand"));

	//MovementComponent->SetMovementMode(MOVE_Falling); // 보통 벽에서 떨어지면 낙하 상태로 둡니다.
	//MovementComponent->bOrientRotationToMovement = true;

	//OwningCharacter->SetCanClimb(false);
	//SetbIsClimbStand(false);

	//OwningCharacter->SetbUseFTimerHandle();

	if (!OwningCharacter || !MovementComponent) return;

	// 1. 상태 변수 즉시 해제 (재실행 방지)
	SetbIsClimbStand(false);
	SetbIsClimb(false);

	// 2. 캐릭터를 약간 앞(벽 안쪽)과 위로 이동시켜서 확실히 안착시키기
	// 이동 거리(PushDistance)는 캐릭터 캡슐 반지름(약 42)보다 조금 더 큰 값이 안전합니다.
	float PushDistance = 100.f;
	float UpOffset = 10.f; // 바닥에 발이 살짝 묻히지 않게 위로도 살짝 올림

	FVector Forward = OwningCharacter->GetActorForwardVector();
	FVector NewLocation = OwningCharacter->GetActorLocation() + (Forward * PushDistance) + (FVector::UpVector * UpOffset);

	// Teleport를 true로 설정하여 물리 엔진 충돌을 무시하고 강제 이동
	OwningCharacter->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);

	// 3. 이동 모드 변경
	MovementComponent->SetMovementMode(MOVE_Walking); // 보정이 끝났으므로 바로 Walking으로 시도
	MovementComponent->bOrientRotationToMovement = true;

	// 4. 쿨타임 및 타이머
	
	OwningCharacter->SetbUseFTimerHandle();

	UE_LOG(LogTemp, Log, TEXT("EndClimb: Teleported character inward to prevent falling."));
}

void UBaseAnimInstance::SetbIsClimb(bool value)
{
	bIsClimb = value;
}

void UBaseAnimInstance::SetbIsClimbStand(bool value)
{
	bIsClimbStand = value;
}

void UBaseAnimInstance::SetClimbInputXY(FVector2D value)
{
	ClimbInputX = value.X;
	ClimbInputY = value.Y;
}

bool UBaseAnimInstance::GetIsClimb()
{
	return bIsClimb;
}

bool UBaseAnimInstance::GetIsClimbStand()
{
	return bIsClimbStand;
}
