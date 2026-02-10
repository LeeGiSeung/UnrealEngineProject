// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget/AutoDialogueWidget/AutoDialogueWidget.h"
#include "DialogueType/DialogueType.h"
#include "EngineUtils.h"


void UAutoDialogueWidget::NativeConstruct()
{
    Super::NativeConstruct();

    for (TActorIterator<ADialogueManager> It(GetWorld()); It; ++It)
    {
        DialogueManager = *It;
        break;
    }

    check(DialogueManager); // 없으면 바로 알 수 있게

    
    AutoRow = DialogueManager->GetAutoRow();

    TextTime += AutoRow->FirstText.ToString().Len();
    TextTime += AutoRow->SecondText.ToString().Len();

    UE_LOG(LogTemp, Warning, TEXT("%f"), TextTime);
}

void UAutoDialogueWidget::CheckAutoDialogueTime(float DeltaTime)
{
    CurAutoTime += DeltaTime * 3;

    UE_LOG(LogTemp, Warning, TEXT("%f"), CurAutoTime);

    if (CurAutoTime >= TextTime) {
        UE_LOG(LogTemp, Warning, TEXT("Next Dialogue"));
        CurAutoTime = 0.f;
        DialogueManager->StartDialogue(AutoRow->NextID, AutoRow->UIType);
    }
}
