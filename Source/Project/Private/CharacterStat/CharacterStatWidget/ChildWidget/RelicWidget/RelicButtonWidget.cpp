// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicListWidget/Relic_List_Widget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


FReply URelicButtonWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
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
	if (InOperation == nullptr) return false;

	URelicButtonWidget* DraggedData = Cast<URelicButtonWidget>(InOperation->Payload);

	if (DraggedData)
	{
		// 3. 내용물이 확인됐으니 처리!
		//RelicList에 event 보냄
		DropRelicDropClicked.Broadcast(DraggedData, this);


		if (Relic_List_Widget) {
			//UE_LOG(LogTemp, Error, TEXT("StartWidge"));
			Relic_List_Widget->HandleRelicDrop(DraggedData, this);
		}



		//this, InOperation 보내서 두개의 data 바꾸면됨
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
		UE_LOG(LogTemp, Error, TEXT("NO RELIC IMAGE"));
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

		//UE_LOG(LogTemp, Error, TEXT("Clicked Widget: %s / Parent: %p"),
		//	*GetName(),
		//	ParentWidget);

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