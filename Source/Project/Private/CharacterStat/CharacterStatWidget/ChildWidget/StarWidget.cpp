// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/StarWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/StarWidget/StarButtonWidget.h"
#include "ServerInfo/Starinfo/FStarinfo.h"

void UStarWidget::NativeConstruct()
{
	StarButton[0] = Star_0;
	StarButton[1] = Star_1;
	StarButton[2] = Star_2;
	StarButton[3] = Star_3;
	StarButton[4] = Star_4;
}


void UStarWidget::UpdateWithServerData(const FStarinfo& Data)
{
	iMaxStarLevel = Data.StarLevel;

	TurnOnStarImage(0);
}

void UStarWidget::TurnOnStarImage(int value)
{
	if (value > iMaxStarLevel) return;

	StarButton[value]->SetStarImage(StarTextures[value]);

	TurnOnStarImage(++value);
}

