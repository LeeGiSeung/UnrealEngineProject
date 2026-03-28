// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseWidget.generated.h"

class UTextBlock;

UCLASS()
class PROJECT_API UBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void UpdateCharacterData();

	virtual void WidgetSetText(UTextBlock* Target, float Value);
	virtual void WidgetSetText(UTextBlock* Target, float Value, FString etc);

};
