// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "Relic_Content_Widget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECT_API URelic_Content_Widget : public UBaseWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HP_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Attack_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Defence_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Force_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Critical_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CriticalDamage_Content;
};
