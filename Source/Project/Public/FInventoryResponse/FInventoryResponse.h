// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerInfo/Relicinfo/RelicData.h"
#include "FInventoryResponse.generated.h"

USTRUCT(BlueprintType)
struct FInventoryResponse : public FTableRowBase {

	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FRelicData> InventoryRelics;

};
