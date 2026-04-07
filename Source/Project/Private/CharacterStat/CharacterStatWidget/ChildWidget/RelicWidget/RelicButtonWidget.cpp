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
}

void URelicButtonWidget::OpenRelicListWidget()
{

	if (RelicWidget && !RelicWidget->GetRelic_List_Widget())
	{
		RelicWidget->RelicWidgetAddToViewPort();
	}
	else {
		if (!ParentWidget) return;
		ParentWidget->UseParentFunction();
	}

}

void URelicButtonWidget::SetRelicWidget(URelicWidget* value)
{
	RelicWidget = value;
}

bool URelicButtonWidget::GetbOpenRelicListWidget()
{
	return bOpenRelicListWidget;
}

void URelicButtonWidget::SetbOpenRelicListWidget(bool value)
{
	bOpenRelicListWidget = value;
}
