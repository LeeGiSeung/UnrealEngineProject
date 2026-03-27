// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStat/CharacterStatWidget/BaseWidget/BaseWidget.h"
#include "ECharacterMenuState/ECharacterMenuState.h"
#include "WindowWidget.generated.h"

class ACharacterStat;

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

	UPROPERTY()
	ACharacterStat* CharacterStat;
};
