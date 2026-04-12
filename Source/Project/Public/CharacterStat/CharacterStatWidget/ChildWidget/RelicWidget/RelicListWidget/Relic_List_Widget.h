// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "ServerInfo/Relicinfo/RelicData.h"
#include "FInventoryResponse/FInventoryResponse.h"

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"

#include "Relic_List_Widget.generated.h"

class URelicButtonWidget;
class URelicWidget;
class URelic_Content_Widget;

/**
 * 
 */
UCLASS()
class PROJECT_API URelic_List_Widget : public UBaseWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
	URelic_Content_Widget* RelicContent;

	UPROPERTY(meta = (BindWidget))
	URelic_Content_Widget* SelectRelicContent;

	URelicWidget* RelicWidget;
	void ReferenceRelicWidget(URelicWidget* value);

	virtual void NativeConstruct() override;

	void SettingRelicButton();

	virtual void UseParentFunction() override;

	UFUNCTION(BlueprintCallable, Category = "Close")
	void CloseWidget();

	URelicButtonWidget* SelectRelicWidget;

	UFUNCTION()
	void OnRelicSelected(URelicButtonWidget* ClickedWidget);

	UPROPERTY()
	TArray<FRelicData> InventoryRelics;

	void OnInventoryResponse(
		FHttpRequestPtr Request,
		FHttpResponsePtr Response,
		bool bWasSuccessful);

	void RequestInventory();

	void UpdateInventoryUI();

	UFUNCTION()
	void HandleRelicDrop(URelicButtonWidget* StartWidget, URelicButtonWidget* EndWidget);

	void PostRelicInventory();
	//void RelicInventoryStruct(FHttpRequestPtr Request,
	//	FHttpResponsePtr Response,
	//	bool bWasSuccessful);


	void PostWearingRelic();

//BindWidget
public:
	void SetWearRelicWidget(TArray<URelicButtonWidget*> value);

	TArray<URelicButtonWidget*> RelicButton_Array;
	TArray<URelicButtonWidget*> WearRelicButton_Array;

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

};
