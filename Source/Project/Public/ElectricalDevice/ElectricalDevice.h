// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "GameplayTagContainer.h"
#include "ElectricalDevice.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API AElectricalDevice : public ABaseActor
{
	GENERATED_BODY()

public:
	AElectricalDevice();

	virtual void Use_Function();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameplayTags")
	FGameplayTagContainer GameplayTags;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UMeshComponent* RenderMesh;
	
};
