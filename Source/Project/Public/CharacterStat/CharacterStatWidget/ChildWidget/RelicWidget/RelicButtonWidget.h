// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "ServerInfo/Relicinfo/RelicData.h"

#include "RelicButtonWidget.generated.h"

class UImage;
class UTexture2D;
class URelic_List_Widget;
class URelicWidget;
class UBorder;

UCLASS()
class PROJECT_API URelicButtonWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelicClicked, URelicButtonWidget*, ClickedWidget);

	UPROPERTY(BlueprintAssignable)
	FOnRelicClicked OnRelicClicked;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDropRelicDropClicked, URelicButtonWidget*, StartWidget, URelicButtonWidget*, EndWidget);

	UPROPERTY(BlueprintAssignable)
	FDropRelicDropClicked DropRelicDropClicked;

public:
	UPROPERTY()
	URelic_List_Widget* Relic_List_Widget;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:

	virtual void NativeConstruct() override;

	void SetRelicImage(UTexture2D* Image);

	void SetRelicTexture2D(UTexture2D* value);
	UTexture2D* GetRelicTexture2D();

	UTexture2D* RelicTexture2D;

	UFUNCTION(BlueprintCallable, Category = "OpenRelic")
	void OpenRelicListWidget();

	UPROPERTY(meta = (BindWidget))
	UImage* RelicImage;

	UPROPERTY(meta = (BindWidget))
	UBorder* SelectBoarder;

	void SetSelectBoarderVistity(ESlateVisibility value);

	void SetRelicWidget(URelicWidget* value);
	URelicWidget* RelicWidget;

	UPROPERTY()
	FRelicData RelicData;

};
