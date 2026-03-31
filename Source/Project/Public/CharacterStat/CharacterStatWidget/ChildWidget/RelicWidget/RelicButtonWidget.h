// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "RelicButtonWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECT_API URelicButtonWidget : public UBaseWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UImage* RelicImage;

};
