// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "City/CitySystemStruct/CitySystemStruct.h"

#include "MapMarkerBase.generated.h"

struct FMapMarkerData;

UCLASS()
class PROJECT_API UMapMarkerBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void MarkerClick();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "MapMarkerData")
	FMapMarkerData MapMarkerData;
	

};
