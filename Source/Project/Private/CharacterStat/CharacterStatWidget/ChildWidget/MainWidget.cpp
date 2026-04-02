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

}

void UMainWidget::UpdateWithServerData(const FMaininfo& Data)
{
	WidgetSetText(HP_Content, Data.HP);
	WidgetSetText(Attack_Content, Data.Attack);
	WidgetSetText(Defence_Content, Data.Defence);
	WidgetSetText(Force_Content, Data.Force, FString("%"));
	WidgetSetText(Critical_Content, Data.Critical, FString("%"));
	WidgetSetText(CriticalDamage_Content, Data.CriticalDamage, FString("%"));
}
