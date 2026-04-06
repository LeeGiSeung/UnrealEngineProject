// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "Components/TextBlock.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStat.h"
#include "Kismet/GameplayStatics.h"


void UBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CharacterStat = Cast<ACharacterStat>(UGameplayStatics::GetActorOfClass(GetWorld(), ACharacterStat::StaticClass()));
}

void UBaseWidget::WidgetSetText(UTextBlock* Target, float Value)
{

	if (Target) Target->SetText(FText::AsNumber(Value));
}

void UBaseWidget::WidgetSetText(UTextBlock* Target, float Value, FString etc)
{
	if (!Target) return;

	FString CombinedString = FString::SanitizeFloat(Value) + etc;

	Target->SetText(FText::FromString(CombinedString));
}

void UBaseWidget::WidgetSetText(UTextBlock* Target, int Value, FString etc)
{
	if (!Target) return;

	FString CombinedString = FString::Printf(TEXT("%d%s"), Value, *etc);

	Target->SetText(FText::FromString(CombinedString));
}

void UBaseWidget::SetMainWidgetPointer(UMainWidget* _MainWidgetPointer)
{
	MainWidgetPointer = _MainWidgetPointer;
}

FMaininfo UBaseWidget::MakeMaininfoStruct(float HP, float Attack, float Defence, float Force, float Critical, float CriticalDamage)
{
	FMaininfo result;

	result.HP = HP;
	result.Attack = Attack;
	result.Defence = Defence;
	result.Force = Force;
	result.Critical = Critical;
	result.CriticalDamage = CriticalDamage;

	return result;
}
