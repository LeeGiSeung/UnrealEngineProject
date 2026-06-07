// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapViewer.generated.h"

class UImage;

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

//Var
public:
	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	TArray<UTexture2D*> MapImageArray;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 NowScollLevel = 16; //스크롤 레벨

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 minScollLevel = 8;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 maxScollLevel = 16;

	UPROPERTY(BlueprintReadWrite, Category = "WheelDownClick")
	bool bMouseWheelDownClick = false;

	UPROPERTY(meta = (BindWidget))
	UImage* MapImage;

};
