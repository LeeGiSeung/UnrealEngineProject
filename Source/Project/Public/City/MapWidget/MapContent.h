// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "City/CitySystemStruct/CitySystemStruct.h"

#include "MapContent.generated.h"

struct FMapMarkerData;

class UCityMapWidget;

UCLASS()
class PROJECT_API UMapContent : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintCallable, Category = "MapContent")
	void SetMainHUD(UCityMapWidget* value);

	TArray<FMapMarkerData> MarkerDataArray;

	UPROPERTY(BlueprintReadWrite, Category = "MapContent")
	UCityMapWidget* CityMapWidget;

};
