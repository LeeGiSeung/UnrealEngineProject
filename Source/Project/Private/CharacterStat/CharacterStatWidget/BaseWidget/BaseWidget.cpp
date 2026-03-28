// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "Components/TextBlock.h"

void UBaseWidget::UpdateCharacterData()
{
	UE_LOG(LogTemp, Error, TEXT("CharacterData Update"));
}

void UBaseWidget::WidgetSetText(UTextBlock* Target, float Value)
{

	if (Target) Target->SetText(FText::AsNumber(Value));
}

void UBaseWidget::WidgetSetText(UTextBlock* Target, float Value, FString etc)
{
	if (!Target) return;

	FString CombinedString = FString::SanitizeFloat(Value) + etc;

	Target->SetText(FText::FromString(CombinedString));
}
