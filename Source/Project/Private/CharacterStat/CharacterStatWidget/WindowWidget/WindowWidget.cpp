// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/WindowWidget/WindowWidget.h"
#include "CharacterStat/CharacterStat.h"
#include "Kismet/GameplayStatics.h"

void UWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterStat = Cast<ACharacterStat>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterStat::StaticClass()));

}

void UWindowWidget::ChangeWindowWidget(ECharacterMenuState MenuState)
{
	if (!CharacterStat) return;

	CharacterStat->SwitchCameraComponent(MenuState);
}
