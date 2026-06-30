// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "City/MapWidget/Marker/MapViewer/PointMarker.h"
#include "Blueprint/UserWidget.h"
#include "MapMarkerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMapMarkerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_API IMapMarkerInterface
{
	GENERATED_BODY()


public:
	virtual TSubclassOf<UUserWidget> GetMarkerClass() const = 0;
	virtual UPointMarker* GetPointMarker() const = 0;
	virtual void SetPointMarker(UPointMarker* Value) = 0;

};
