// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "City/CitySystemStruct/CitySystemStruct.h"

#include "CityMapWidget.generated.h"

class UUCityNewworkManager;
class UMapContent;
class UMapViewer;

UCLASS()
class PROJECT_API UCityMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DelataTime) override;

	UFUNCTION(BlueprintCallable, Category = "Navigation")
	void NavigationFunction();

	UFUNCTION(BlueprintCallable, Category = "Mapviewer")
	void MapMarkerIndexUp();

	UFUNCTION(BlueprintCallable, Category = "Mapviewer")
	void MapMarkerIndexDown();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void SetMapViewerToMainHUD();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void SetMapContentToMainHUD();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void OnPaintMapViewer();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Navigation")
	UUCityNewworkManager* CityNewworkManager;

	UPROPERTY(BlueprintReadWrite, Category = "HUDSpawnWidget")
	TSubclassOf<UMapViewer> MapViewer;

	UPROPERTY(BlueprintReadWrite, Category = "HUDSpawnWidget")
	TSubclassOf<UMapContent> MapContent;

	UPROPERTY(meta = (BindWidget))
	UMapViewer* WBP_MapViewer;

	UPROPERTY(meta = (BindWidget))
	UMapContent* WBP_MapContent;

private:

};
