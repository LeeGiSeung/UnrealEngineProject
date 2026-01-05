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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack")
	bool IsSpecialAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpecialAttack")
	UAnimMontage* SpecialAttackMontage;

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack")
	void PlaySpecialAttackMontage(ACharacter* APlayer);

	UFUNCTION()
	void AnimNotify_EndSpecialAttack(UAnimNotify* Notify);

};
