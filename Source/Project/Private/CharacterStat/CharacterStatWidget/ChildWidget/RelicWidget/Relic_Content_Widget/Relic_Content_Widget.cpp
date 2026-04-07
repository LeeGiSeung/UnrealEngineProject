// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/Relic_Content_Widget/Relic_Content_Widget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget/RelicButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStat.h"

void URelic_Content_Widget::SetRelicStat(const FRelicData Data)
{
	ContentInfo.Attack += Data.Attack;
	ContentInfo.Defence += Data.Defence;
	ContentInfo.Force += Data.Force;
	ContentInfo.HP += Data.HP;
	ContentInfo.Critical += Data.Critical;
	ContentInfo.CriticalDamage += Data.CriticalDamage;
}

void URelic_Content_Widget::SetRelicWidgetContentText()
{
	WidgetSetText(HP_Content, ContentInfo.HP);
	WidgetSetText(Attack_Content, ContentInfo.Attack);
	WidgetSetText(Defence_Content, ContentInfo.Defence);
	WidgetSetText(Force_Content, ContentInfo.Force, FString("%"));
	WidgetSetText(Critical_Content, ContentInfo.Critical, FString("%"));
	WidgetSetText(CriticalDamage_Content, ContentInfo.CriticalDamage, FString("%"));
}
