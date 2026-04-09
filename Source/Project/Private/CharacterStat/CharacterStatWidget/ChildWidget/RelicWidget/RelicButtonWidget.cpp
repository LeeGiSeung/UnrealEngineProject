// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicListWidget/Relic_List_Widget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "Components/Image.h"

void URelicButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();



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

void URelicButtonWidget::SetRelicWidget(URelicWidget* value)
{
	RelicWidget = value;
}