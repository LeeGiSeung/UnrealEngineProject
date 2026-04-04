// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/StarWidget/StarButtonWidget.h"
#include "Components/Image.h"

void UStarButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UStarButtonWidget::SetStarImage(UTexture2D* value)
{
	StarImage->SetBrushFromTexture(value);
}
