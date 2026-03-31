// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "FSkillInfo/FSkillInfo.h"

#include "SkillButtonWidget.generated.h"

class UImage;
class UTexture2D;

/**
 * 
 */
UCLASS()
class PROJECT_API USkillButtonWidget : public UBaseWidget
{
	GENERATED_BODY()
	

public:

	void SettingSkillLevel(int value);

	virtual void NativeConstruct() override;

	void ChangeSkillText();
	void SettingNodeImage(bool value);
	void SettingSkillImage(UTexture2D* texture);

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
