// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TogetherRunAnimInstance.generated.h"

class ATogetherRunBase;
/**
 * 
 */
UCLASS()
class PROJECT_API UTogetherRunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:


public:

	void SetfTogetherAnimGroundSpeed(float value);
	void SetfTogetherAnimShouldMove(bool value);

	UPROPERTY(BlueprintReadWrite, Category = "Essential Movement Data")
	float fGroundSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Movement Data")
	bool bShouldMove;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Movement Data")
	bool bTogetherChild;

	UPROPERTY(BlueprintReadOnly, Category = "CoupleRun")
	FVector LeftHandLocation;

	UPROPERTY(BlueprintReadOnly, Category = "CoupleRun")
	FRotator LeftHandRotation;

	UPROPERTY(BlueprintReadOnly, Category = "CoupleRun")
	FVector LocalFVector;

	UPROPERTY(BlueprintReadOnly, Category = "CoupleRun")
	FVector JointTarget;

	UPROPERTY()
	ATogetherRunBase* NPC;

	void SetHandLocation(FVector Location);
	void SetNPCReference(ATogetherRunBase* value);
};
