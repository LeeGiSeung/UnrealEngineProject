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

//#Server
public:
	FSkillInfo SkillData;

	void OnSkillDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void UpdateWithServerData(const FSkillInfo& Data);
	void RequestSkillDataFromServer();

	UFUNCTION(BlueprintCallable, Category = "Post")
	void TestGet();

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
