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
	UTextBlock* GetUpTextBlock() { return TextBlockUp; }
	void SetUpText(FText _value) { TextBlockUp->SetText(_value); }

	UTextBlock* GetMiddleTextBlock() { return TextBlockMiddle; }
	void SetMiddleText(FText _value) { TextBlockMiddle->SetText(_value); }

	UTextBlock* GetDownTextBlock() { return TextBlockDown; }
	void SetDownText(FText _value) { TextBlockDown->SetText(_value); }

	void SetUpEmpty() { TextBlockUp->SetText(FText::GetEmpty()); }
	void SetMiddleEmpty() { TextBlockMiddle->SetText(FText::GetEmpty()); }
	void SetDownEmpty() { TextBlockDown->SetText(FText::GetEmpty()); }

//CALLBACKFUNCTION
public:
	UFUNCTION()
	virtual void OnFinishedPlayDialogueSound();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockUp;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockMiddle;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextBlockDown;

};
