// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricalDevice/MyElectricalDevice_Door.h"
#include "etc/GameplayTag.h"

AMyElectricalDevice_Door::AMyElectricalDevice_Door()
{

}

void AMyElectricalDevice_Door::BeginPlay()
{
	//if (GameplayTags.HasTag(TAG_Electrical))
}

void AMyElectricalDevice_Door::Use_Function()
{
	Super::Use_Function();

	UE_LOG(LogTemp, Warning, TEXT("UseFun"));
}