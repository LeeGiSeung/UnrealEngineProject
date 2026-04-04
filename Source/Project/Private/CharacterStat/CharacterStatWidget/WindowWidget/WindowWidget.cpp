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

	UE_LOG(LogTemp, Error, TEXT("Spawn MainWidget"));

	CharacterStat->FindWidgetClass(BP_SkillWidget, BP_MainWidget, BP_RelicWidget, BP_StarWidget);

	UE_LOG(LogTemp, Error, TEXT("Spawn WindowWidget"));

	SetMainWidgetPointer(BP_MainWidget);
}

void UWindowWidget::ChangeWindowWidget(ECharacterMenuState MenuState)
{
	if (!CharacterStat) return;

	CharacterStat->SwitchCameraComponent(MenuState);
}
