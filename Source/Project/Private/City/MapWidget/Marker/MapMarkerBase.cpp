// Fill out your copyright notice in the Description page of Project Settings.


#include "City/MapWidget/Marker/MapMarkerBase.h"


void UMapMarkerBase::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UMapMarkerBase::MarkerClick()
{
	UE_LOG(LogTemp, Error, TEXT("MapMarkerBase Click"));
}
