// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/WindowWidget/WindowWidget.h"
#include "CharacterStat/CharacterStat.h"
#include "Kismet/GameplayStatics.h"

//Widget
#include "CharacterStat/CharacterStatWidget/ChildWidget/SkillWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/StarWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void UWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterStat = Cast<ACharacterStat>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterStat::StaticClass()));

	//USkillWidget* Skill, UMainWidget* Main, URelicWidget* Relic, UStarWidget* Star

	CharacterStat->FindWidgetClass(BP_SkillWidget, BP_MainWidget, BP_RelicWidget, BP_StarWidget);
}

void UWindowWidget::ChangeWindowWidget(ECharacterMenuState MenuState)
{
	if (!CharacterStat) return;

	CharacterStat->SwitchCameraComponent(MenuState);
}
