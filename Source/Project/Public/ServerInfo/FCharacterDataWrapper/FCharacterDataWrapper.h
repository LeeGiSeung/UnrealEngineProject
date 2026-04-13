// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ServerInfo/Relicinfo/FRelicinfo.h"
#include "ServerInfo/Maininfo/FMaininfo.h"
#include "ServerInfo/Starinfo/FStarinfo.h"
#include "FSkillInfo/FSkillInfo.h"

#include "FCharacterDataWrapper.generated.h"

USTRUCT(BlueprintType)
struct FCharacterDataWrapper : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerId;

	UPROPERTY()
	FString CharacterId;

	UPROPERTY()
	FString CharacterName;

	UPROPERTY()
	FMaininfo Maininfo;

	UPROPERTY()
	FRelicinfo Relicinfo;

	UPROPERTY()
	FSkillInfo SkillInfo;
	
	UPROPERTY()
	FStarinfo StarInfo;
};
