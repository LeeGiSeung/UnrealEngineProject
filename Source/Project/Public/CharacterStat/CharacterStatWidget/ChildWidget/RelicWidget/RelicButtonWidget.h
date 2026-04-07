// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "RelicButtonWidget.generated.h"

class UImage;
class UTexture2D;
class URelic_List_Widget;
class URelicWidget;

UCLASS()
class PROJECT_API URelicButtonWidget : public UBaseWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct() override;

	void SetRelicImage(UTexture2D* Image);

	UFUNCTION(BlueprintCallable, Category = "OpenRelic")
	void OpenRelicListWidget();

	UPROPERTY(meta = (BindWidget))
	UImage* RelicImage;

	void SetRelicWidget(URelicWidget* value);
	URelicWidget* RelicWidget;

	URelic_List_Widget* RelicListWidget;
	bool bOpenRelicListWidget = false;
	bool GetbOpenRelicListWidget();
	void SetbOpenRelicListWidget(bool value);



};
