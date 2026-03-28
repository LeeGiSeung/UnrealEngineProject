// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "Components/TextBlock.h"

void UMainWidget::UpdateCharacterData()
{
	Super::UpdateCharacterData();

	UE_LOG(LogTemp, Error, TEXT("MainWidget UpdateData"));
}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	float a = 240;

	WidgetSetText(HP_Content, 6521);
	WidgetSetText(Attack_Content, 2700);
	WidgetSetText(Defence_Content, 1200);
	WidgetSetText(Force_Content, 130, FString("%"));
	WidgetSetText(Critical_Content, 87, FString("%"));
	WidgetSetText(CriticalDamage_Content, a, FString("%"));



}
