// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "City/MapWidget/Marker/MapMarkerBase.h"
#include "PointMarker.generated.h"

class UMapViewer;

/**
 * 
 */
UCLASS()
class PROJECT_API UPointMarker : public UMapMarkerBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void MarkerClick() override;

	UFUNCTION()
	void SetSpawnMarker(int32 value, UMapViewer* viewer);

	UFUNCTION(BlueprintCallable, Category = "PointMarker")
	void DeletePointMarker();

public:
	UPROPERTY()
	UMapViewer* MapViewer;

private:
	int SpawnMarkerIdx = 0;

};
