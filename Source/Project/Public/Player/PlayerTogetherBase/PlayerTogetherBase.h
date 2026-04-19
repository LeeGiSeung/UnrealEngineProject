// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectCharacter.h"
#include "PlayerTogetherBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API APlayerTogetherBase : public AProjectCharacter
{
	GENERATED_BODY()
	
	APlayerTogetherBase();

public:
	virtual void BeginPlay() override;
	virtual void Tick(float Tick) override;

};
