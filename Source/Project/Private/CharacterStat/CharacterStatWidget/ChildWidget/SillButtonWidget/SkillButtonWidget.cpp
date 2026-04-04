// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/SillButtonWidget/SkillButtonWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/SkillWidget.h"
#include "Components/Image.h"

void USkillButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void USkillButtonWidget::SettingSkillLevel(int value, USkillWidget* _SkillWidget)
{

	SkillWidget = _SkillWidget;

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

void USkillButtonWidget::SettingNodeImage(bool value, USkillWidget* _SkillWidget)
{
	UClass* CurrentClass = GetClass();

	SkillWidget = _SkillWidget;

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

void USkillButtonWidget::SettingSkillImage(UTexture2D* texture, USkillWidget* _SkillWidget)
{
	SkillWidget = _SkillWidget;

	SkillImage->SetBrushFromTexture(texture);
}

void USkillButtonWidget::SelectButton()
{
	if (!SkillWidget) {
		UE_LOG(LogTemp, Error, TEXT("NO SkillWidget"));
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("YES SkillWidget"));
	}
}
