// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "SkillButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API USkillButtonWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

};
