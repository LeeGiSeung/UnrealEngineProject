// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicListWidget/Relic_List_Widget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

//UMG 위젯 이벤트 오버라이딩해서 해결 어차피 reutrn 값들이 전부 이벤트 들이여서 굳이 super은 안해도됨
FReply URelicButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!GetRelicTexture2D()) return FReply::Unhandled();

	OpenRelicListWidget();
	
	return UWidgetBlueprintLibrary::DetectDragIfPressed(
		InMouseEvent, this, EKeys::LeftMouseButton
	).NativeReply;
}

void URelicButtonWidget::NativeOnDragDetected(
	const FGeometry& InGeometry,
	const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{

	UDragDropOperation* DragOp = NewObject<UDragDropOperation>();

	// 어떤 위젯이 드래그됐는지 전달
	DragOp->Payload = this;

	// 드래그 중 원본 숨기기
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	// 드래그 중 보일 UI
	DragOp->DefaultDragVisual = this;

	OutOperation = DragOp;
}

bool URelicButtonWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	// 1. 엔진이 들고 온 상자(InOperation)가 비어있는지 확인
	if (InOperation == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("NO INOPERATION"));
		return false;
	}
	URelicButtonWidget* DraggedData = Cast<URelicButtonWidget>(InOperation->Payload);

	if (DraggedData)
	{
		DropRelicDropClicked.Broadcast(DraggedData, this);


		if (Relic_List_Widget) {
			//UE_LOG(LogTemp, Error, TEXT("StartWidge"));
			if (!DraggedData->GetRelicTexture2D()) return false;
			//Relic_List_Widget->HandleRelicDrop(DraggedData, this);
		}

		return true;
	}

	return false;
}

void URelicButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectBoarder->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

}

void URelicButtonWidget::SetRelicImage(UTexture2D* Image)
{
	if (!Image) {
		RelicImage->SetBrushFromTexture(nullptr);
		SetRelicTexture2D(nullptr);
		return;
	}
	RelicImage->SetBrushFromTexture(Image);
	SetRelicTexture2D(Image);
}

void URelicButtonWidget::SetRelicTexture2D(UTexture2D* value)
{
	RelicTexture2D = value;
}

UTexture2D* URelicButtonWidget::GetRelicTexture2D()
{
	return RelicTexture2D;
}

void URelicButtonWidget::OpenRelicListWidget()
{

	if (RelicWidget && !RelicWidget->GetRelic_List_Widget())
	{
		RelicWidget->RelicWidgetAddToViewPort();
	}
	else {
		if (!ParentWidget) {
			UE_LOG(LogTemp, Error, TEXT("NO PAR"));
			return;
		}

		OnRelicClicked.Broadcast(this);
	}

}

void URelicButtonWidget::SetSelectBoarderVistity(ESlateVisibility value)
{
	SelectBoarder->SetVisibility(value);
}

void URelicButtonWidget::SetRelicWidget(URelicWidget* value)
{
	RelicWidget = value;
}