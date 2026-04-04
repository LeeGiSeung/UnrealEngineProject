// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/SillButtonWidget/SkillButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/SkillWidget.h"
#include "Components/Image.h"

void USkillButtonWidget::SetSkillDataKey(FString value)
{
	SkillDataKey = value;
}

FString USkillButtonWidget::GetSkillDataKey()
{
	return SkillDataKey;
}

void USkillButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillButtonWidget::SettingSkillLevel(int value)
{

	SettingNodeImage(true);

	CurSkillLevel = FMath::Clamp(value, 1, MaxSkillLevel);

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

void USkillButtonWidget::SettingSkillWidget(USkillWidget* value)
{
	SkillWidget = value;
}

void USkillButtonWidget::SelectButton()
{
	if (!SkillWidget) return;

	SkillWidget->RecoardSkillButtonWidget(this);
}
