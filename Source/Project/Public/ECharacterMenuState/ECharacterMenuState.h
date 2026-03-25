// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ECharacterMenuState.generated.h"

UENUM(BlueprintType)
enum class ECharacterMenuState : uint8 {
	Main,
	Relic,
	Skill,
	Star
};