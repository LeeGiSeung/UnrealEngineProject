// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "FSkillInfo/FSkillInfo.h"

#include "SkillButtonWidget.generated.h"

class UImage;
class UTexture2D;
class USkillWidget;

/**
 * 
 */
UCLASS()
class PROJECT_API USkillButtonWidget : public UBaseWidget
{
	GENERATED_BODY()
	

public:

	void SetSkillDataKey(FString value);
	FString GetSkillDataKey();

	FString SkillDataKey;

	virtual void NativeConstruct() override;

	void ChangeSkillText();

	void SettingSkillLevel(int value);
	int GetSkillLevel() {
		return CurSkillLevel
			;
	}
	void SettingNodeImage(bool value);
	void SettingSkillImage(UTexture2D* texture);

	void SettingSkillWidget(USkillWidget* value);

	UFUNCTION(BlueprintCallable, Category = "SkillLevel")
	void SelectButton();

	UPROPERTY()
	USkillWidget* SkillWidget;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SkillLevel;

	UPROPERTY(meta = (BindWidget))
	UImage* SkillImage;

	UPROPERTY(BlueprintReadWrite, Category = "NodeImage")
	UTexture2D* TrueImage;

	UPROPERTY(BlueprintReadWrite, Category = "NodeImage")
	UTexture2D* FalseImage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Level")
	float MaxSkillLevel;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Skill Level")
	float CurSkillLevel = 1;

};
