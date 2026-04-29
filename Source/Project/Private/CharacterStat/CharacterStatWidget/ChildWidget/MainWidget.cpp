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

	MainInfo.Level = 1;

}

void UMainWidget::UpdateWithServerData(const FMaininfo& Data)
{

	if (!CharacterStat) {
		return;
	}


	FMaininfo maininfo = CharacterStat->GetMainStat();

	//CharacterStat->SetMainStat(Data);

	//maininfo = CharacterStat->GetMainStat();

	UE_LOG(LogTemp, Error, TEXT("Data.Level %f"), Data.Level);

	Subinfo.HP += Data.HP;
	Subinfo.Defence += Data.Defence;
	Subinfo.Attack += Data.Attack;
	Subinfo.Force += Data.Force;
	Subinfo.Critical += Data.Critical;
	Subinfo.CriticalDamage += Data.CriticalDamage;

	MainInfo.LevelEXP += Data.LevelEXP;
	MainInfo.Level += Data.Level;

	WidgetSetText(HP_Content, Subinfo.HP + maininfo.HP);
	WidgetSetText(Attack_Content, Subinfo.Attack + maininfo.Attack);
	WidgetSetText(Defence_Content, Subinfo.Defence + maininfo.Defence);
	WidgetSetText(Force_Content, Subinfo.Force + maininfo.Force, FString("%"));
	WidgetSetText(Critical_Content, Subinfo.Critical + maininfo.Critical, FString("%"));
	WidgetSetText(CriticalDamage_Content, Subinfo.CriticalDamage + maininfo.CriticalDamage, FString("%"));
	WidgetSetText(LVTextBlock, 1, FString(" .LV")); //오버라이드

	EXPBar->SetPercent(maininfo.LevelEXP + Data.LevelEXP);
	
}

FMaininfo UMainWidget::GetMainInfo()
{
	ResetSubinfo();
	return MainInfo;
}

void UMainWidget::ResetSubinfo()
{
	Subinfo.HP = 0.f;
	Subinfo.Defence = 0.f;
	Subinfo.Attack = 0.f;
	Subinfo.Force = 0.f;
	Subinfo.Critical = 0.f;
	Subinfo.CriticalDamage = 0.f;
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
