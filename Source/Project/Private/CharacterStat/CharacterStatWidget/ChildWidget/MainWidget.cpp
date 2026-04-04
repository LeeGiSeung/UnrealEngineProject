// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStat.h"
#include "Components/TextBlock.h"

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
	
	if (!CharacterStat) return;

	FMaininfo maininfo = CharacterStat->GetMainStat();

	CharacterStat->SetMainStat(Data);

	maininfo = CharacterStat->GetMainStat();

	WidgetSetText(HP_Content, maininfo.HP);
	WidgetSetText(Attack_Content, maininfo.Attack);
	WidgetSetText(Defence_Content, maininfo.Defence);
	WidgetSetText(Force_Content, maininfo.Force, FString("%"));
	WidgetSetText(Critical_Content, maininfo.Critical, FString("%"));
	WidgetSetText(CriticalDamage_Content, maininfo.CriticalDamage, FString("%"));
}
