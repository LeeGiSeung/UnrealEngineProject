// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapButton.generated.h"

class UButton;
class UImage;

UCLASS()
class PROJECT_API UMapButton : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetMapButtonImage(UTexture2D* value);

public:
	UPROPERTY(meta = (Bindwidget), EditAnywhere)
	UButton* Button;

	UPROPERTY(meta = (Bindwidget), EditAnywhere)
	UImage* Image;

protected:
	UFUNCTION(BlueprintCallable, Category = "MapButton")
	virtual void ClickMapButton();
};
