// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "StarButtonWidget.generated.h"

class UImage;
class UTexture2D;

/**
 * 
 */
UCLASS()
class PROJECT_API UStarButtonWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetStarImage(UTexture2D* value);

	UPROPERTY(meta = (BindWidget))
	UImage* StarImage;

};
