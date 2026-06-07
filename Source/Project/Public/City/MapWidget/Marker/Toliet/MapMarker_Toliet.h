// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/MapWidget/Marker/MapMarkerBase.h"
#include "MapMarker_Toliet.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMapMarker_Toliet : public UMapMarkerBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void MarkerClick() override;

};
