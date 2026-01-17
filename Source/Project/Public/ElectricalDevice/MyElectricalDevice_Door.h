// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ElectricalDevice/ElectricalDevice.h"
#include "GameplayTagContainer.h"
#include "MyElectricalDevice_Door.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AMyElectricalDevice_Door : public AElectricalDevice
{
	GENERATED_BODY()
	
public:
	AMyElectricalDevice_Door();
	virtual void Use_Function() override;

	virtual void BeginPlay() override;

};
