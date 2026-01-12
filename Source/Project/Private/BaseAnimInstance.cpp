// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "GameFramework/Character.h"          
#include "Components/SkeletalMeshComponent.h" 
#include "Animation/AnimInstance.h"
#include "GameFramework/PlayerController.h"


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
