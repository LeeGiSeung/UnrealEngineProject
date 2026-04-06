// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStat.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void UMainWidget::UpdateCharacterData()
{
	Super::UpdateCharacterData();
}

void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UMainWidget::UpdateWithServerData(const FMaininfo& Data)
{
	
	if (Data.Level <= 0 || Data.Level > 10000)
	{
		return;
	}

	if (!CharacterStat) return;

	FMaininfo maininfo = CharacterStat->GetMainStat();

	CharacterStat->SetMainStat(Data);

	maininfo = CharacterStat->GetMainStat();

	MainInfo = maininfo;

	WidgetSetText(HP_Content, maininfo.HP);
	WidgetSetText(Attack_Content, maininfo.Attack);
	WidgetSetText(Defence_Content, maininfo.Defence);
	WidgetSetText(Force_Content, maininfo.Force, FString("%"));
	WidgetSetText(Critical_Content, maininfo.Critical, FString("%"));
	WidgetSetText(CriticalDamage_Content, maininfo.CriticalDamage, FString("%"));
	WidgetSetText(LVTextBlock, Data.Level, FString(" .LV")); //오버라이드 된거임

	EXPBar->SetPercent(Data.LevelEXP);
	
}

FMaininfo UMainWidget::GetMainInfo()
{
	return MainInfo;
}

void UMainWidget::LevelUp()
{
	if (MainInfo.Level > 90.f) {
		EXPBar->SetPercent(1.f);
		return;
	}
	float localPercent = EXPBar->GetPercent();
	EXPBar->SetPercent(localPercent + 0.1f);

	if (EXPBar->GetPercent() >= 1.f) {

		EXPBar->SetPercent(0);
		MainInfo.Level += 1;

		WidgetSetText(LVTextBlock, MainInfo.Level, FString(" .Lv"));

		MainInfo.HP += 100;
		MainInfo.Attack += 30;
		MainInfo.Defence += 50;
		
		WidgetSetText(HP_Content, MainInfo.HP);
		WidgetSetText(Attack_Content, MainInfo.Attack);
		WidgetSetText(Defence_Content, MainInfo.Defence);

	}

	MainInfo.LevelEXP = EXPBar->GetPercent();

	CharacterStat->SendSkillUpgradeToServer();

}
