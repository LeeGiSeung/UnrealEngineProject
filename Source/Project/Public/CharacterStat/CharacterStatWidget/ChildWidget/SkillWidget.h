// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "FSkillInfo/FSkillInfo.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"

#include "SkillWidget.generated.h"

class USkillButtonWidget;
class UTexture;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECT_API USkillWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void UpdateCharacterData() override;

	void SkillToMainStat(FSkillInfo Data);

	UFUNCTION(BlueprintCallable, Category = "SkillLevelUp")
	void SkillLevelUp();
	
	UFUNCTION(BlueprintCallable, Category = "SkillLevelUp")
	void RecoardSkillButtonWidget(USkillButtonWidget* value);

	UPROPERTY()
	USkillButtonWidget* LastRecoardSkillButtonWidget;

	UPROPERTY()
	FSkillInfo skillInfo;

//#Server
public:

	void UpdateWithServerData(const FSkillInfo& Data);

	UFUNCTION(BlueprintCallable, Category = "Post")
	void TestGet();

	UPROPERTY(meta = (BindWidget))
	UButton* LevelUpButton;

public:
	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* AutoAttackWidget;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* Auto_0;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* Auto_1;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* Auto_2;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* E_0;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* E_1;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* E_2;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* P_0;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* P_1;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* P_2;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* R_0;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* R_1;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* R_2;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* EAttackWidget;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* RAttackWidget;

	UPROPERTY(meta = (BindWidget))
	USkillButtonWidget* PAttackWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillButtonImage")
	UTexture2D* AutoAttackImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillButtonImage")
	UTexture2D* EAttackImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillButtonImage")
	UTexture2D* RAttackImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "USkillButtonImage")
	UTexture2D* PAttackImage;

};
