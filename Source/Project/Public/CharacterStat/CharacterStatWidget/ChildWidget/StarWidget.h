// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "ServerInfo/Starinfo/FStarinfo.h"

#include "StarWidget.generated.h"

class UStarButtonWidget;

/**
 * 
 */
UCLASS()
class PROJECT_API UStarWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	FStarinfo StarInfo;
	FStarinfo GetStarInfo();

	void UpdateWithServerData(const FStarinfo& Data);

	void TurnOnStarImage(int value);

	virtual void NativeConstruct() override;

	int iMaxStarLevel;

	UPROPERTY(BlueprintReadWrite, Category = "StarTexture")
	TArray<UTexture2D*> StarTextures;

	UPROPERTY()
	UStarButtonWidget* StarButton[5];

	UPROPERTY(meta = (BindWidget))
	UStarButtonWidget* Star_0;

	UPROPERTY(meta = (BindWidget))
	UStarButtonWidget* Star_1;

	UPROPERTY(meta = (BindWidget))
	UStarButtonWidget* Star_2;

	UPROPERTY(meta = (BindWidget))
	UStarButtonWidget* Star_3;

	UPROPERTY(meta = (BindWidget))
	UStarButtonWidget* Star_4;
};
