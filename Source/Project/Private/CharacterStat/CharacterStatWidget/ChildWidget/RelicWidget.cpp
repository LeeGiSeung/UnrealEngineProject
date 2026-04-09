// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStat.h"
#include "Components/WidgetSwitcher.h"

#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/Relic_Content_Widget/Relic_Content_Widget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicListWidget/Relic_List_Widget.h"

FRelicinfo URelicWidget::GetRelicInfo()
{
	return RelicInfo;
}

void URelicWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RelicArray.Add(Relic_0);
	RelicArray.Add(Relic_1);
	RelicArray.Add(Relic_2);
	RelicArray.Add(Relic_3);
	RelicArray.Add(Relic_4);

	for (URelicButtonWidget* Widget : RelicArray)
	{
		if (Widget)
		{
			Widget->SetRelicWidget(this);
		}
	}

}

void URelicWidget::UpdateWithServerData(const FRelicinfo& Data)
{

	RelicInfo = Data;

	SettingRelicButtonImage(Data.Part_0, 0);
	SettingRelicButtonImage(Data.Part_1, 1);
	SettingRelicButtonImage(Data.Part_2, 2);
	SettingRelicButtonImage(Data.Part_3, 3);
	SettingRelicButtonImage(Data.Part_4, 4);

	RelicToMainStat(Data.Part_0);
	RelicToMainStat(Data.Part_1);
	RelicToMainStat(Data.Part_2);
	RelicToMainStat(Data.Part_3);
	RelicToMainStat(Data.Part_4);

	RelicAllContent->SetRelicStat(Data.Part_0);
	RelicAllContent->SetRelicStat(Data.Part_1);
	RelicAllContent->SetRelicStat(Data.Part_2);
	RelicAllContent->SetRelicStat(Data.Part_3);
	RelicAllContent->SetRelicStat(Data.Part_4);

	RelicAllContent->SetRelicWidgetContentText();
	
}

void URelicWidget::SettingRelicButtonImage(FRelicData Data, int WidgetIndex)
{
	FGameplayTag Tag = FNameChangeToTag(Data.RelicImageId);
	if (Tag.IsValid())
	{
		if (UTexture2D** Found = RelicMap.Find(Tag))
		{
			if (!RelicArray[WidgetIndex]) return;
			RelicArray[WidgetIndex]->SetRelicImage(*Found);
			RelicArray[WidgetIndex]->RelicData = Data;
		}
	}
}

void URelicWidget::RelicToMainStat(FRelicData Data)
{
	FMaininfo LocalInfo = MakeMaininfoStruct(
		Data.HP,
		Data.Attack,
		Data.Defence,
		Data.Force,
		Data.Critical,
		Data.CriticalDamage
	);

	if (!CharacterStat->GetMainWidget()) return;
	
	CharacterStat->GetMainWidget()->UpdateWithServerData(LocalInfo);
}

FGameplayTag URelicWidget::FNameChangeToTag(FName value)
{
	FGameplayTag Tag;

	if (value == FName()) return Tag;

	Tag = FGameplayTag::RequestGameplayTag(
		FName(value),
		false // 없으면 크래시 방지
	);

	return Tag;
}

void URelicWidget::ChangeRelicWidget()
{

}

void URelicWidget::RelicWidgetAddToViewPort()
{
	if (Relic_List_Widget || !Relic_List_Widget_Class) return;

	Relic_List_Widget = CreateWidget<URelic_List_Widget>(this, Relic_List_Widget_Class);
	Relic_List_Widget->AddToViewport();
	Relic_List_Widget->SetWearRelicWidget(RelicArray);
	Relic_List_Widget->ReferenceRelicWidget(this);
}

URelic_List_Widget* URelicWidget::GetRelic_List_Widget()
{
	return Relic_List_Widget;
}

void URelicWidget::RemoveRelic_List_Widget()
{
	Relic_List_Widget = nullptr;
}
