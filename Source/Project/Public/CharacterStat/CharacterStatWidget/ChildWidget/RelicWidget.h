// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "ServerInfo/Relicinfo/FRelicinfo.h"
#include "GameplayTagContainer.h"
#include "ServerInfo/Relicinfo/RelicData.h"

#include "RelicWidget.generated.h"

class Texture2D;
class URelicButtonWidget;

/**
 * 
 */
UCLASS()
class PROJECT_API URelicWidget : public UBaseWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void UpdateWithServerData(const FRelicinfo& Data);

	void SettingRelicButtonImage(FRelicData Data, int WidgetIndex);

	void RelicToMainStat(FRelicData Data);

	FGameplayTag FNameChangeToTag(FName value);

	UPROPERTY(EditAnywhere ,BlueprintReadWrite, Category = "RelicMap")
	TMap< FGameplayTag, UTexture2D*> RelicMap;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* Relic_0;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* Relic_1;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* Relic_2;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* Relic_3;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* Relic_4;

	UPROPERTY()
	URelicButtonWidget* RelicArray[5];

};
