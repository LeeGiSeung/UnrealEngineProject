// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CityMapWidget.generated.h"

class UUCityNewworkManager;

UCLASS()
class PROJECT_API UCityMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DelataTime) override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void NavigationFunction();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Navigation")
	UUCityNewworkManager* CityNewworkManager;

private:

};
