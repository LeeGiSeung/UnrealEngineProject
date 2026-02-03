#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DialogueType/DialogueType.h"
#include "FDialogueRow.generated.h"

USTRUCT(BlueprintType)
struct FDialogueRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Speaker;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText FirstText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText SecondText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Choice1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Choice2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Choice3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Choice4;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName NextID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDialogueUIType UIType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName EventKey;
};