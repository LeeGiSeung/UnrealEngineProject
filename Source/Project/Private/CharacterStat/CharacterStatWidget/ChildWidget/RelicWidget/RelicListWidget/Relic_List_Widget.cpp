// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicListWidget/Relic_List_Widget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "CharacterStat/CharacterStat.h"

void URelic_List_Widget::ReferenceRelicWidget(URelicWidget* value)
{
	RelicWidget = value;
}

void URelic_List_Widget::NativeConstruct()
{
	Super::NativeConstruct();

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

	for (URelicButtonWidget* widget : RelicButton_Array) {
		if (widget) {
			UE_LOG(LogTemp, Error, TEXT("widget"));
			widget->SetParentWidget(this);
		}
	}

	UE_LOG(LogTemp, Error, TEXT("NativeConstruct"));

}

void URelic_List_Widget::UseParentFunction()
{

}
