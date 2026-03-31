// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/SillButtonWidget/SkillButtonWidget.h"
#include "Components/Image.h"

void USkillButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillButtonWidget::SettingSkillLevel(int value)
{

	CurSkillLevel = FMath::Clamp(value,1,MaxSkillLevel);

	ChangeSkillText();

}

void USkillButtonWidget::ChangeSkillText()
{
	UClass* CurrentClass = GetClass();

	if (CurrentClass) {
		FString ClassName = CurrentClass->GetName();
		if (ClassName == FString("WBP_SkillButtonWidget_C")) {
			FString MaxLevel = FString::SanitizeFloat(MaxSkillLevel);
			WidgetSetText(SkillLevel, CurSkillLevel, FString(" / " + MaxLevel));
		}
	}
}

void USkillButtonWidget::SettingNodeImage(bool value)
{
	UClass* CurrentClass = GetClass();

	if (CurrentClass) {
		FString ClassName = CurrentClass->GetName();
		if (ClassName == FString("WBP_NoText_SkillButtonWidget_C")) {
			if (value) {
				SkillImage->SetBrushFromTexture(TrueImage);
			}
			else {
				SkillImage->SetBrushFromTexture(FalseImage);
			}
		}
	}
}

void USkillButtonWidget::SettingSkillImage(UTexture2D* texture)
{
	SkillImage->SetBrushFromTexture(texture);
}
