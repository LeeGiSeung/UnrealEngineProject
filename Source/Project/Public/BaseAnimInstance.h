// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"
class UAnimMontage;
/**
 * 
 */

UCLASS()
class PROJECT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	DECLARE_MULTICAST_DELEGATE(FOnSpecialAttackFinished);
	FOnSpecialAttackFinished OnSpecialAttackFinished;

	DECLARE_MULTICAST_DELEGATE(FOnCrouchBackFinished);
	FOnCrouchBackFinished OnCrouchBackFinished;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack")
	bool IsSpecialAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack")
	UAnimMontage* SpecialAttackMontage;

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack")
	void PlaySpecialAttackMontage(ACharacter* APlayer);

	UFUNCTION()
	void AnimNotify_EndSpecialAttack(UAnimNotify* Notify);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrouchBack")
	bool IsCrouchBack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CrouchBack")
	UAnimMontage* CrouchBackMontage;

	UFUNCTION(BlueprintCallable, Category = "CrouchBack")
	void PlayCrouchBackMontage(ACharacter* APlayer);

	UFUNCTION(Category = "CrouchBack")
	void AnimNotify_EndCrouchBack(UAnimNotify* Notify);

};
