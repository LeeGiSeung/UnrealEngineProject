// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseUserWidget.generated.h"

class UTextureRenderTarget2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDrawFinished);

/**
 * 
 */
UCLASS()
class PROJECT_API UBaseUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "CanvasCapture")
	void SaveCanvasRenderTargetToPNG(UTextureRenderTarget2D* Canvas, const FString& FilePath, const FString& FileName);

	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void RecordPosition(FVector2D Pos);

	UPROPERTY(BlueprintReadWrite, Category = "Drawing")
	TArray<FVector2D> DrawPositions;

	UPROPERTY(BlueprintAssignable, Category = "Drawing")
	FOnDrawFinished OnDrawFinished;

	UFUNCTION(BlueprintCallable)
	void FinishDrawing();
};
