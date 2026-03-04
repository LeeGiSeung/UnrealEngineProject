// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnergyWidget.generated.h"

class UProgressBar;

/**
 * 
 */
UCLASS()
class PROJECT_API UEnergyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Category = "EnergyBar")
	UProgressBar* EnergyBar;

};
