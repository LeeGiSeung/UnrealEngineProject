// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicListWidget/Relic_List_Widget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/Relic_Content_Widget/Relic_Content_Widget.h"
#include "CharacterStat/CharacterStat.h"
#include "Components/Image.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "ServerInfo/Relicinfo/RelicData.h"


#include "JsonObjectConverter.h"

void URelic_List_Widget::ReferenceRelicWidget(URelicWidget* value)
{
	RelicWidget = value;
}

void URelic_List_Widget::NativeConstruct()
{
	Super::NativeConstruct();

	SettingRelicButton();

	RequestInventory();

}

void URelic_List_Widget::SettingRelicButton()
{
	RelicButton_Array.Add(RelicList_0);
	RelicButton_Array.Add(RelicList_1);
	RelicButton_Array.Add(RelicList_2);
	RelicButton_Array.Add(RelicList_3);
	RelicButton_Array.Add(RelicList_4);
	RelicButton_Array.Add(RelicList_5);
	RelicButton_Array.Add(RelicList_6);
	RelicButton_Array.Add(RelicList_7);
	RelicButton_Array.Add(RelicList_8);
	RelicButton_Array.Add(RelicList_9);
	RelicButton_Array.Add(RelicList_10);
	RelicButton_Array.Add(RelicList_11);

	WearRelicButton_Array.Add(Relic_0);
	WearRelicButton_Array.Add(Relic_1);
	WearRelicButton_Array.Add(Relic_2);
	WearRelicButton_Array.Add(Relic_3);
	WearRelicButton_Array.Add(Relic_4);

	for (URelicButtonWidget* widget : RelicButton_Array) {
		if (widget) {
			widget->ParentWidget = this;
		}
	}

	for (URelicButtonWidget* widget : WearRelicButton_Array) {
		if (widget) {
			widget->ParentWidget = this;
		}
	}

	for (auto* Item : RelicButton_Array)
	{
		Item->OnRelicClicked.AddDynamic(this, &URelic_List_Widget::OnRelicSelected);
		Item->DropRelicDropClicked.AddDynamic(this, &URelic_List_Widget::HandleRelicDrop);

		Item->Relic_List_Widget = this;
	}

	for (auto* Item : WearRelicButton_Array)
	{
		Item->OnRelicClicked.AddDynamic(this, &URelic_List_Widget::OnRelicSelected);
		Item->DropRelicDropClicked.AddDynamic(this, &URelic_List_Widget::HandleRelicDrop);

		Item->Relic_List_Widget = this;
	}
}

void URelic_List_Widget::UseParentFunction()
{
	if (!ParentWidget) return;

}

void URelic_List_Widget::CloseWidget()
{
	if (!RelicWidget) return;
	RemoveFromParent();
	RelicWidget->RemoveRelic_List_Widget();
}

void URelic_List_Widget::SetWearRelicWidget(TArray<URelicButtonWidget*> value)
{
	for (int i = 0; i < WearRelicButton_Array.Num(); i++) {
		WearRelicButton_Array[i]->RelicData = value[i]->RelicData;
		WearRelicButton_Array[i]->RelicImage->SetBrushFromTexture(value[i]->GetRelicTexture2D());
		WearRelicButton_Array[i]->SetRelicTexture2D(value[i]->GetRelicTexture2D());
		
		RelicContent->SetRelicStat(WearRelicButton_Array[i]->RelicData);
	}

	RelicContent->SetRelicWidgetContentText();
}

void URelic_List_Widget::OnRelicSelected(URelicButtonWidget* ClickedWidget)
{
	if (SelectRelicWidget) { //이미 selectwidget이 있을때
		SelectRelicWidget->SetSelectBoarderVistity(ESlateVisibility::SelfHitTestInvisible);
	}
	

	SelectRelicWidget = ClickedWidget;
	
	SelectRelicContent->ResetRelicStat();
	SelectRelicContent->SetRelicStat(SelectRelicWidget->RelicData);
	SelectRelicContent->SetRelicWidgetContentText();

	ClickedWidget->SetSelectBoarderVistity(ESlateVisibility::Visible);

}

void URelic_List_Widget::RequestInventory()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	Request->SetURL("http://127.0.0.1:3000/api/relics/player_01");
	Request->SetVerb("GET");

	Request->OnProcessRequestComplete().BindUObject(
		this,
		&URelic_List_Widget::OnInventoryResponse
	);

	Request->ProcessRequest();
}

void URelic_List_Widget::OnInventoryResponse(
	FHttpRequestPtr Request,
	FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Request Failed"));
		return;
	}

	FString JsonString = Response->GetContentAsString();

	FInventoryResponse Data;

	if (FJsonObjectConverter::JsonObjectStringToUStruct(
		JsonString,
		&Data,
		0, 0))
	{
		InventoryRelics = Data.InventoryRelics;

		//UE_LOG(LogTemp, Warning, TEXT("Inventory Loaded: %d"), InventoryRelics.Num());

		UpdateInventoryUI();
	}
}

void URelic_List_Widget::UpdateInventoryUI()
{

	for (int i = 0; i < InventoryRelics.Num(); i++) {
		if (RelicButton_Array.Num() <= i) break; //더이상 못채우면 break

		RelicButton_Array[i]->RelicData = InventoryRelics[i];

		FGameplayTag tag = RelicWidget->FNameChangeToTag(InventoryRelics[i].RelicImageId);

		if (UTexture2D** Found = RelicWidget->RelicMap.Find(tag))
		{
			RelicButton_Array[i]->SetRelicImage(*Found);
		}
	}
}

void URelic_List_Widget::HandleRelicDrop(URelicButtonWidget* StartWidget, URelicButtonWidget* EndWidget)
{
	//교환

	//UE_LOG(LogTemp, Error, TEXT("StartWidget : %s, EndWidget : %s"), *StartWidget->GetName(), *EndWidget->GetName());

	UE_LOG(LogTemp, Warning, TEXT("=== Before (Before Swap) ==="));
	UE_LOG(LogTemp, Log, TEXT("StartWidget: ID=%s"), *StartWidget->RelicData.RelicImageId.ToString());
	UE_LOG(LogTemp, Log, TEXT("EndWidget  : ID=%s"), *EndWidget->RelicData.RelicImageId.ToString());

	FRelicData StartData = StartWidget->RelicData;
	UTexture2D* StartTexture2D = StartWidget->GetRelicTexture2D();

	FRelicData EndData = EndWidget->RelicData;;
	UTexture2D* EndTexture2D = EndWidget->GetRelicTexture2D();

	StartWidget->SetRelicImage(EndTexture2D);
	StartWidget->RelicData = EndData;

	EndWidget->SetRelicImage(StartTexture2D);
	EndWidget->RelicData = StartData;

	UE_LOG(LogTemp, Warning, TEXT("=== After (After Swap) ==="));
	UE_LOG(LogTemp, Log, TEXT("StartWidget: ID=%s"), *StartWidget->RelicData.RelicImageId.ToString());
	UE_LOG(LogTemp, Log, TEXT("EndWidget  : ID=%s"), *EndWidget->RelicData.RelicImageId.ToString());

	//InventoryRelics 여기에 접근해서 자체를 바꿔줘야함 InventoryRelics는 이전 RelicWidget의 포인터를 잡고있음
	int32 StartIndex = RelicButton_Array.Find(StartWidget);
	int32 EndIndex = RelicButton_Array.Find(EndWidget);

	if (StartIndex != INDEX_NONE && EndIndex != INDEX_NONE)
	{
		InventoryRelics.Swap(StartIndex, EndIndex);
	}

	PostRelicInventory();
	PostWearingRelic();
}

void URelic_List_Widget::PostRelicInventory()
{
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	FInventoryResponse InventoryResponse;

	for (int i = 0; i < RelicButton_Array.Num(); i++) {
		InventoryResponse.InventoryRelics.Add(
			RelicButton_Array[i]->RelicData
		);
	}

	// JSON 변환
	FString JsonString;
	if (!FJsonObjectConverter::UStructToJsonObjectString(InventoryResponse, JsonString))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to serialize Inventory"));
		return;
	}

	// 요청 세팅
	Request->SetURL(TEXT("http://127.0.0.1:3000/api/relics/player_01"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetContentAsString(JsonString);

	// 응답 처리
	Request->OnProcessRequestComplete().BindLambda(
		[](FHttpRequestPtr Req, FHttpResponsePtr Res, bool bSuccess)
		{
			if (!bSuccess || !Res.IsValid())
			{
				UE_LOG(LogTemp, Error, TEXT("POST Failed"));
				return;
			}
		}
	);

	Request->ProcessRequest();
}

void URelic_List_Widget::PostWearingRelic()
{
	//RelicWidget으로 현재 Wearing Widget 보내기
	RelicWidget->ChangeRelicArray(&WearRelicButton_Array);

	//CharacterStat에서 다시 넘기기
	if (!CharacterStat)
	{
		UE_LOG(LogTemp, Error, TEXT("NO CHARACTERSTAT"));
		return;
	}

}
