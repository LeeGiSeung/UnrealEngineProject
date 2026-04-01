// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Datatable.h"
#include "FRelicinfo.generated.h"

USTRUCT(BlueprintType)
struct FRelicinfo : public FTableRowBase {

	GENERATED_BODY()

	UPROPERTY()
	FName Part_1;

	UPROPERTY()
	FName Part_2;

	UPROPERTY()
	FName Part_3;

	UPROPERTY()
	FName Part_4;

	UPROPERTY()
	FName Part_5;

};
