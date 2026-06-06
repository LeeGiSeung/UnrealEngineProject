// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/MapWidget/MapButton/MapButton.h"
#include "MapButton_Brush.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMapButton_Brush : public UMapButton
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	virtual void ClickMapButton() override;

};
