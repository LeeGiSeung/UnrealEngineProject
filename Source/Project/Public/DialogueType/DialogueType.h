// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDialogueUIType : uint8
{
    Normal,
    Choice,
    Auto,
    Cinematic,
    End
};

UENUM(BlueprintType)
enum class EDialogueEventType : uint8
{
    Normal,
    CameraSlowAroundMove
};
