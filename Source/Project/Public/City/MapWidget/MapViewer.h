// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapViewer.generated.h"

class UImage;
class UPointMarker;
class UCanvasPanel;

UCLASS()
class PROJECT_API UMapViewer : public UUserWidget
{
	GENERATED_BODY()

//Function
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "MapViewFunction")
	void ScroolUp();

	UFUNCTION(BlueprintCallable, Category = "MapViewFunction")
	void ScroolDown();

	UFUNCTION(BlueprintCallable, Category = "MapViewFunction")
	void MapMove(FVector2D value);

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void ChangeMapImage();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void MapRightButtonClick(const FVector2D& MousePosition);

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void MapLeftButtonClick();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void MousePositionToImagePath(const FVector2D& _MousePosition);

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void SpawnMapPointMarker(const FVector2D& _MousePosition);

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void DeleteMapPointMarker(UPointMarker* marker);

	UFUNCTION(BlueprintCallable)
	void UpMapMarkerMaxCount();

	UFUNCTION(BlueprintCallable)
	void DownMapMarkerMaxCount();

public:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MapViewerCanvasPanel;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	TSubclassOf<UPointMarker> PointMarkerClass;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	TArray<UTexture2D*> MapImageArray;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 NowScollLevel = 16; //스크롤 레벨

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 minScollLevel = 13;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 maxScollLevel = 18;

	UPROPERTY(BlueprintReadWrite, Category = "WheelDownClick")
	bool bMouseWheelDownClick = false;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapViewer")
	UImage* MapImage;
	 
	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 iMapMarkerCount = 0;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 iMapMarkerMaxCount = 2;

	UPROPERTY()
	TArray<UPointMarker*> PointMarkerArray;

	UFUNCTION()
	void TestiMapMarkerMaxCount();

};
