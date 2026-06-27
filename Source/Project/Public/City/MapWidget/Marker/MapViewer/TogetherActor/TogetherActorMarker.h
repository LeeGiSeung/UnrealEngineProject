// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/MapWidget/Marker/MapViewer/PointMarker.h"
#include "TogetherActorMarker.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECT_API UTogetherActorMarker : public UPointMarker
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UImage* TogetherActorImage;

	UFUNCTION()
	void TurnTogetherImage(FRotator value);

};
