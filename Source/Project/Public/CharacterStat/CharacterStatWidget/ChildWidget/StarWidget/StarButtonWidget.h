// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "StarButtonWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECT_API UStarButtonWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* StarImage;

};
