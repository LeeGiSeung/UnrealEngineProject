// Fill out your copyright notice in the Description page of Project Settings.


#include "City/MapWidget/MapButton/MapButton.h"

void UMapButton::NativeConstruct()
{
	Super::NativeConstruct();

}

void UMapButton::SetMapButtonImage(UTexture2D* value)
{

}

void UMapButton::ClickMapButton()
{
	if (!Button) {
		UE_LOG(LogTemp, Error, TEXT("NO Button"));
	}
	UE_LOG(LogTemp, Error, TEXT("ClickMapButton"));
}
