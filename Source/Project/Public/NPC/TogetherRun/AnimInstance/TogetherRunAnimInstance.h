// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "TogetherRunAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UTogetherRunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Essential Movement Data")
	float fGroundSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Essential Movement Data")
	bool bShouldMove;

};
