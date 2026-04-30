// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "ServerInfo/MainInfo/FMaininfo.h"


#include "MainWidget.generated.h"

class UTextBlock;
class UProgressBar;
class UButton;
/**
 * 
 */
UCLASS()
class PROJECT_API UMainWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	virtual void UpdateCharacterData() override;

	virtual void NativeConstruct() override;

	void UpdateWithServerData(const FMaininfo& Data);

	FMaininfo MainInfo;

	FMaininfo Subinfo;

	FMaininfo maininfo;

	FMaininfo GetMainInfo();

	void ResetSubinfo();

public:
	UFUNCTION(BlueprintCallable, Category = "MainLevelUp")
	void LevelUp();

	UPROPERTY()
	int Level;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LVTextBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* EXPLevelUpButton;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* EXPBar;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HP_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Attack_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Defence_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Force_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Critical_Content;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CriticalDamage_Content;

};
