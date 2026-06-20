// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/MapWidget/Marker/MapViewer/PointMarker.h"
#include "PersonMarker.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UPersonMarker : public UPointMarker
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

};
