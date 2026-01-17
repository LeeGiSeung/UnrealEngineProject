// Fill out your copyright notice in the Description page of Project Settings.


#include "ElectricalDevice/ElectricalDevice.h"


AElectricalDevice::AElectricalDevice()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	
	GameplayTags.AddTag(FGameplayTag::RequestGameplayTag("Electrical"));

}

void AElectricalDevice::Use_Function()
{
}

