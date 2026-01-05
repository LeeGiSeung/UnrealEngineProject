// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimInstance.h"
#include "GameFramework/Character.h"          
#include "Components/SkeletalMeshComponent.h" 
#include "Animation/AnimInstance.h"           


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
		UE_LOG(LogTemp, Warning, TEXT("Special Attack Montage Played"));
	}

}

void UBaseAnimInstance::AnimNotify_EndSpecialAttack(UAnimNotify* Notify)
{
	IsSpecialAttack = false;
	UE_LOG(LogTemp, Warning, TEXT("Special Attack Finished"));
}
