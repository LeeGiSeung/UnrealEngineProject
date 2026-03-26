// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "StatAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UStatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlendSpace")
	float BlendSpaceX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "BlendSpace")
	float BlendSpaceY;

	UFUNCTION(BlueprintCallable, Category = "BlendSpace")
	void SetBlendSpace(float X, float Y);

};
