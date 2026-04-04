// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "ECharacterMenuState/ECharacterMenuState.h"
#include "WindowWidget.generated.h"

class ACharacterStat;
class USkillWidget;
class URelicWidget;
class UStarWidget;
class UMainWidget;

/**
 * 
 */
UCLASS()
class PROJECT_API UWindowWidget : public UBaseWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "WindowWidget")
	void ChangeWindowWidget(ECharacterMenuState MenuState);

	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	USkillWidget* BP_SkillWidget;

	UPROPERTY(meta = (BindWidget))
	UMainWidget* BP_MainWidget;

	UPROPERTY(meta = (BindWidget))
	UStarWidget* BP_StarWidget;

	UPROPERTY(meta = (BindWidget))
	URelicWidget* BP_RelicWidget;

};
