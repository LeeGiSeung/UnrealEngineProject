// Fill out your copyright notice in the Description page of Project Settings.


#include "City/MapWidget/MapButton/Erase/MapButton_Erase.h"

void UMapButton_Erase::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapButton_Erase::ClickMapButton()
{
	Super::ClickMapButton();

	UE_LOG(LogTemp, Error, TEXT("Erase Button"));
}
