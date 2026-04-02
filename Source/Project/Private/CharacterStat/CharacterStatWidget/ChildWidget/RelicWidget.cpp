// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"


void URelicWidget::NativeConstruct()
{
	Super::NativeConstruct();

	RelicArray[0] = Relic_0;
	RelicArray[1] = Relic_1;
	RelicArray[2] = Relic_2;
	RelicArray[3] = Relic_3;
	RelicArray[4] = Relic_4;

}

void URelicWidget::UpdateWithServerData(const FRelicinfo& Data)
{
	SettingRelicButton(Data.Part_0, 0);
	SettingRelicButton(Data.Part_1, 1);
	SettingRelicButton(Data.Part_2, 2);
	SettingRelicButton(Data.Part_3, 3);
	SettingRelicButton(Data.Part_4, 4);
}

void URelicWidget::SettingRelicButton(FRelicData Data, int WidgetIndex)
{
	FGameplayTag Tag = FNameChangeToTag(Data.RelicImageId);
	if (Tag.IsValid())
	{
		if (UTexture2D** Found = RelicMap.Find(Tag))
		{
			if (!RelicArray[WidgetIndex]) return;
			RelicArray[WidgetIndex]->SetRelicImage(*Found);
		}
	}
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
