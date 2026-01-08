// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UTextureRenderTarget2D;

/**
 * 
 */
UCLASS()
class PROJECT_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category = "CanvasCapture")
	void SaveCanvasRenderTargetToPNG(UTextureRenderTarget2D* Canvas, const FString& FilePath, const FString& FileName);

};
