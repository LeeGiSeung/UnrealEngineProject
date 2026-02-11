// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget/ChoiceDialogueWidget/ChoiceDialogueWidget.h"
#include "DialogueType/DialogueType.h"
#include "EngineUtils.h"

void UChoiceDialogueWidget::NativeConstruct()
{

    for (TActorIterator<ADialogueManager> It(GetWorld()); It; ++It)
    {
        DialogueManager = *It;
        break;
    }

    check(DialogueManager); // 없으면 바로 알 수 있게

    ProjectPlayerController = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());
    
    ChoiceRow = DialogueManager->GetChoiceRow(); //현재 선택지 Row 가져옴

}

void UChoiceDialogueWidget::PlayerChoiceSelect(int _num)
{
    switch (_num)
    {
    case 0: NextID = ChoiceRow->ChoiceTextAnswer1; break;
    case 1: NextID = ChoiceRow->ChoiceTextAnswer2; break;
    case 2: NextID = ChoiceRow->ChoiceTextAnswer3; break;
    case 3: NextID = ChoiceRow->ChoiceTextAnswer4; break;
    default: return;
    }
    
    //UE_LOG(LogTemp, Warning, TEXT("%s"), *NextID.ToString());

    ProjectPlayerController->bShowMouseCursor = false;

    DialogueManager->StartDialogue(NextID, ChoiceRow->UIType);
}

void UChoiceDialogueWidget::HiideFourChocie()
{
    Button4_1->SetVisibility(ESlateVisibility::Hidden);
    Button4_2->SetVisibility(ESlateVisibility::Hidden);
    Button4_3->SetVisibility(ESlateVisibility::Hidden);
    Button4_4->SetVisibility(ESlateVisibility::Hidden);
}

void UChoiceDialogueWidget::HideThreeChoice()
{
    Button3_1->SetVisibility(ESlateVisibility::Hidden);
    Button3_2->SetVisibility(ESlateVisibility::Hidden);
    Button3_3->SetVisibility(ESlateVisibility::Hidden);
}

void UChoiceDialogueWidget::HideTwoChoice()
{
    Button4_1->SetVisibility(ESlateVisibility::Hidden);
    Button4_4->SetVisibility(ESlateVisibility::Hidden);
    Button3_1->SetVisibility(ESlateVisibility::Hidden);
    Button3_2->SetVisibility(ESlateVisibility::Hidden);
    Button3_3->SetVisibility(ESlateVisibility::Hidden);
}

void UChoiceDialogueWidget::VisibilityFourChoice()
{
    Button4_1->SetVisibility(ESlateVisibility::Visible);
    Button4_2->SetVisibility(ESlateVisibility::Visible);
    Button4_3->SetVisibility(ESlateVisibility::Visible);
    Button4_4->SetVisibility(ESlateVisibility::Visible);
}

void UChoiceDialogueWidget::VisibilityThreeChoice()
{
    Button3_1->SetVisibility(ESlateVisibility::Visible);
    Button3_2->SetVisibility(ESlateVisibility::Visible);
    Button3_3->SetVisibility(ESlateVisibility::Visible);
}

void UChoiceDialogueWidget::VisibilityTwoChoice()
{
    Button4_2->SetVisibility(ESlateVisibility::Visible);
    Button4_3->SetVisibility(ESlateVisibility::Visible);
}

void UChoiceDialogueWidget::SetTwoChoice()
{
    HideTwoChoice();
    VisibilityTwoChoice();
}

void UChoiceDialogueWidget::SetThreeChoice()
{
    HiideFourChocie();
    VisibilityThreeChoice();
}

void UChoiceDialogueWidget::SetFourChoice()
{
    HideThreeChoice();
    VisibilityFourChoice();
}
