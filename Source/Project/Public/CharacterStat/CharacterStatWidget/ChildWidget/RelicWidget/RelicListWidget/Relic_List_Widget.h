// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "Relic_List_Widget.generated.h"

class URelicButtonWidget;
class URelicWidget;

/**
 * 
 */
UCLASS()
class PROJECT_API URelic_List_Widget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	URelicWidget* RelicWidget;
	void ReferenceRelicWidget(URelicWidget* value);

	virtual void NativeConstruct() override;

	virtual void UseParentFunction() override;

//BindWidget
public:
	TArray<URelicButtonWidget*> RelicButton_Array;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_0;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_1;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_2;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_3;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_4;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_5;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_6;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_7;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_8;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_9;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_10;

	UPROPERTY(meta = (BindWidget))
	URelicButtonWidget* RelicList_11;

};
