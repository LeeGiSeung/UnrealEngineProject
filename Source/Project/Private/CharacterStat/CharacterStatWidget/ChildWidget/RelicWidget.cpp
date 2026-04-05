// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStat.h"

FRelicinfo URelicWidget::GetRelicInfo()
{
	return RelicInfo;
}

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

	UE_LOG(LogTemp, Error, TEXT("%f"), Data.Critical);

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
