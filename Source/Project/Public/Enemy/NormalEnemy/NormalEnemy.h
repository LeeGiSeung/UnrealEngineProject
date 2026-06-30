// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/BaseEnemy/BaseEnemy.h"
#include "NormalEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ANormalEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	ANormalEnemy();
	virtual void BeginPlay() override;

};
