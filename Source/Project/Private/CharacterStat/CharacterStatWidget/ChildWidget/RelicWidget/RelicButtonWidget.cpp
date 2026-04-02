// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
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
