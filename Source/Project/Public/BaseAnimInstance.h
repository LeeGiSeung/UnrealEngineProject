// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"
class UAnimMontage;
class AProjectCharacter;
class UCharacterMovementComponent;
/**
 * 
 */

UCLASS()
class PROJECT_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void NativeBeginPlay() override;

	DECLARE_MULTICAST_DELEGATE(FOnSpecialAttackFinished);
	FOnSpecialAttackFinished OnSpecialAttackFinished;

	DECLARE_MULTICAST_DELEGATE(FOnCrouchBackFinished);
	FOnCrouchBackFinished OnCrouchBackFinished;

	AProjectCharacter* OwningCharacter;
	UCharacterMovementComponent* MovementComponent;

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

	UFUNCTION(BlueprintCallable ,Category = "ClimbStand")
	void EndClimbStand();

	void SetbIsClimb(bool value);
	void SetbIsClimbStand(bool value);
	void SetClimbInputXY(FVector2D value);

	bool GetIsClimb();
	bool GetIsClimbStand();

	UPROPERTY(VisibleAnywhere ,BlueprintReadWrite, Category = "Climb")
	bool bIsClimb;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Climb")
	float ClimbInputX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Climb")
	float ClimbInputY;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Climb")
	bool bIsClimbStand;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Falling")
	bool bIsCppFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Falling")
	bool bIsFullFalling;

	bool GetIsCppFalling() {
		return bIsCppFalling;
	}

	void SetIsFullFalling(bool value);

	bool GetIsFullFalling();

	UFUNCTION(BlueprintCallable, Category = "Falling")
	void EndRolling();

};
