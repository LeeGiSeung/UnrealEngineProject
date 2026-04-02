// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include "ServerInfo/Relicinfo/RelicData.h"

#include "FRelicinfo.generated.h"

USTRUCT(BlueprintType)
struct FRelicinfo : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY()
	FRelicData Part_0;

	UPROPERTY()
	FRelicData Part_1;

	UPROPERTY()
	FRelicData Part_2;

	UPROPERTY()
	FRelicData Part_3;

	UPROPERTY()
	FRelicData Part_4;

};
