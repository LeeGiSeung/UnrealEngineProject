// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerInfo/MainInfo/FMaininfo.h"

#include "BaseWidget.generated.h"

class UMainWidget;
class UTextBlock;
class ACharacterStat;

UCLASS()
class PROJECT_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	ACharacterStat* CharacterStat;

	virtual void NativeConstruct() override;

	virtual void UpdateCharacterData() {}

	virtual void WidgetSetText(UTextBlock* Target, float Value);
	virtual void WidgetSetText(UTextBlock* Target, float Value, FString etc);
	virtual void WidgetSetText(UTextBlock* Target, int Value, FString etc);

	UPROPERTY()
	UMainWidget* MainWidgetPointer;

	void SetMainWidgetPointer(UMainWidget* MainWidgetPointer);

	UPROPERTY()
	FMaininfo HandOverFMain;

	FMaininfo MakeMaininfoStruct(float HP, float Attack, float Defence, float Force, float Critical, float CriticalDamage);

	UPROPERTY();
	UBaseWidget* ParentWidget;

	void SetParentWidget(UBaseWidget* value);

public:
	virtual void UseParentFunction();
};
