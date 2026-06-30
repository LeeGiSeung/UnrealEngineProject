// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/MapWidget/Marker/MapViewer/PointMarker.h"
#include "EnemyMarker.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECT_API UEnemyMarker : public UPointMarker
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

};
