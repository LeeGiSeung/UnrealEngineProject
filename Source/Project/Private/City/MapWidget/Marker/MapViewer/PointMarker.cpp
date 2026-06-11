// Fill out your copyright notice in the Description page of Project Settings.


#include "City/MapWidget/Marker/MapViewer/PointMarker.h"
#include "City/MapWidget/MapViewer.h"

void UPointMarker::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPointMarker::MarkerClick()
{
	Super::MarkerClick();
}

void UPointMarker::SetSpawnMarker(int32 value, UMapViewer* viewer)
{
	SpawnMarkerIdx = value;
	MapViewer = viewer;
}

void UPointMarker::DeletePointMarker()
{
	MapViewer->DeleteMapPointMarker(this);
}
