// Fill out your copyright notice in the Description page of Project Settings.


#include "City/MapWidget/MapButton/AllClear/MapButton_AllClear.h"

void UMapButton_AllClear::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMapButton_AllClear::ClickMapButton()
{
	Super::ClickMapButton();

	UE_LOG(LogTemp, Error, TEXT("ClickMapButton"));

}
