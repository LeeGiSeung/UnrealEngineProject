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

    for (TActorIterator<ADirectingManager> It(GetWorld()); It; ++It)
    {
        DirectingManager = *It;
        break;
    }

    if (!DirectingManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("NO DIRECTINGMANAGER"));
    }
    
    AutoRow = DialogueManager->GetAutoRow();

    TextTime += AutoRow->FirstText.ToString().Len();
    TextTime += AutoRow->SecondText.ToString().Len();

    //UE_LOG(LogTemp, Warning, TEXT("%f"), TextTime);
}

void UAutoDialogueWidget::CheckAutoDialogueTime(float DeltaTime)
{
    if (!AutoRow->Speaker.IsNone()) return; //혹시 Speaker가 지정돼있으면 안됨

    CurAutoTime += DeltaTime * 3;

    if (CurAutoTime >= TextTime) {
        CurAutoTime = 0.f;
        DirectingManager->SetLevelSequencePlay(true);
        DialogueManager->StartDialogue(AutoRow->NextID, AutoRow->UIType);
    }
}

void UAutoDialogueWidget::OnFinishedPlayDialogueSound()
{
    UE_LOG(LogTemp, Warning, TEXT("Finish Auto Dialogue"));
    return;
}
