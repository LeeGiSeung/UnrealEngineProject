// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/MapWidget/Marker/MapViewer/PointMarker.h"
#include "BossMarker.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECT_API UBossMarker : public UPointMarker
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

};
