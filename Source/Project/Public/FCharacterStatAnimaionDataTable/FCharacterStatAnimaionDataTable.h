// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" // 반드시 포함해야 FTableRowBase를 인식합니다.
#include "FCharacterStatAnimaionDataTable.generated.h"

class UAnimSequence;

USTRUCT(BlueprintType)
struct FCharacterStatAnimaionDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* MainAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* RelicAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* SkillAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimSequence* StarAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float CharacterAlphaDistance;

};
