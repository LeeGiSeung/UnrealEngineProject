// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "Cable.generated.h"

/**
 * 
 */

class UCableComponent;
class UBoxComponent;

UCLASS()
class PROJECT_API ACable : public ABaseActor
{
	GENERATED_BODY()
	
public:
	ACable();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;

	// 비주얼용 케이블
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCableComponent* Cable;

	// 충돌용 (플레이어 차단)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* CollisionBox;

};
