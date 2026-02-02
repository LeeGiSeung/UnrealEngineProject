// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

#include "BaseDialogueWidget.generated.h"
/**
 * 
 */
UCLASS()
class PROJECT_API UBaseDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UTextBlock* GetTextBlock() { return TextBlock; }
	void SetText(FText _value) { TextBlock->SetText(_value); }

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlock;

};
