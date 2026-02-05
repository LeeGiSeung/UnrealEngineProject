// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget/ChoiceDialogueWidget/ChoiceDialogueWidget.h"
#include "EngineUtils.h"

void UChoiceDialogueWidget::NativeConstruct()
{

    for (TActorIterator<ADialogueManager> It(GetWorld()); It; ++It)
    {
        DialogueManager = *It;
        break;
    }

    check(DialogueManager); // 없으면 바로 알 수 있게
}
