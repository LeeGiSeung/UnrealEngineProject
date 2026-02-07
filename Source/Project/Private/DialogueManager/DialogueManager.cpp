// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager/DialogueManager.h"
#include "DialogueType/DialogueType.h"
#include "Engine/DataTable.h"

#include "EngineUtils.h"
#include "DirectingManager/DirectingManager.h"

//#각 Widget들
#include "DialogueWidget/ChoiceDialogueWidget/ChoiceDialogueWidget.h"
#include "DialogueWidget/NormalDialogueWidget/NormalDialogueWidget.h"
#include "DialogueWidget/BaseDialogueWidget.h"

ADialogueManager::ADialogueManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADialogueManager::BeginPlay()
{
	Super::BeginPlay();

	ProjectPlayerController = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());
}

//외부 Actor로 부터 상호작용해 컷신 시작하기
void ADialogueManager::StartDialogue(FName _ID, EDialogueUIType _Type)
{
	//여기서 csv에 접근해서 ID, eventkey 등 여러개 가져와야함

	ID = _ID; //처음 아이디 지정
	UIType = _Type; //처음 UITYPE 지정
	SetUseIdalogue(true);
	//현재 보여줘야 하는 컷신을 정해줌
	ProjectPlayerController->IgnoreLookMove();

	ShowCurDialogue();
}

//현재 적용돼있는 ID에 해당하는 Dialogue를 보여줌
void ADialogueManager::ShowCurDialogue()
{
	if (!DialogueNormalTable || !DialogueChoiceTable) {
		UE_LOG(LogTemp, Warning, TEXT("No DataTable"));
		ProjectPlayerController->AllowLookMove();
		return;
	}

	if (DialogueWidgetMap.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("No Widget Map"));
		ProjectPlayerController->AllowLookMove();
		return;
	}

	if (CurDialogueWidget) { //현재 사용되고있는 Widget이 있으면 그 위젯을 지우고 새로운 위젯
		RemoveCurDialogueWidget();
	}

	switch (UIType) {
	case EDialogueUIType::Normal:
		
		
		NormalRow = DialogueNormalTable->FindRow<FDialogueRow>(ID, TEXT("DialogueText"));
		
		if (!NormalRow) {
			EndDialogue();
			UE_LOG(LogTemp, Warning, TEXT("No Row"));
			return;
		}

		NextID = NormalRow->NextID;

		break;
		//선택지 넘길 수 있게 추가해야함

	case EDialogueUIType::Choice:
		ChoiceRow = DialogueChoiceTable->FindRow<FChoiceDialogueRow>(ID, TEXT("DialogueText"));

		if (!ChoiceRow) {
			EndDialogue();
			UE_LOG(LogTemp, Warning, TEXT("No Row"));
			return;
		}

		PlayerChoiceNumberCheck();

		break;

	case EDialogueUIType::Auto:

		break;


	case EDialogueUIType::End:
		EndDialogue();
		return;
	}

	CurDialogueWidget = CreateWidget<UBaseDialogueWidget>(GetWorld()->GetFirstPlayerController(), DialogueWidgetMap[UIType]);

	if (CurDialogueWidget) {

		switch (UIType)
		{
		case EDialogueUIType::Normal:
			ChangeCurDialogueWidgetNormalText();
			UIType = NormalRow->UIType;
			
			break;

		case EDialogueUIType::Choice:
			ChangeCurDialogueWidgetChoiceText();
			ChangeCurDialogueWidgetChoice();

			UIType = ChoiceRow->UIType;
			break;

		case EDialogueUIType::Auto:
			break;
		}

		WidgetAddViewPort();
		//각 UiType에 따라서 추가 작업 배정

	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NO WIDGET"));
		return;
	}
	//연출도 해야함 DirectingManager에 연출 row 보내서 연출 시작
}

void ADialogueManager::PlayerChoiceNumberCheck()
{
	switch (iPlayerChoiceNumber)
	{
	case 1:
		NextID = ChoiceRow->ChoiceTextAnswer1;
		break;

	case 2:
		NextID = ChoiceRow->ChoiceTextAnswer2;
		break;

	case 3:
		NextID = ChoiceRow->ChoiceTextAnswer3;
		break;

	case 4:
		NextID = ChoiceRow->ChoiceTextAnswer4;
		break;

	default:
		NextID = ChoiceRow->ChoiceTextAnswer1; //일단 첫번째 걸로 
		break;
	}
}


















void ADialogueManager::WidgetAddViewPort()
{
	CurDialogueWidget->AddToViewport();
}

void ADialogueManager::RemoveCurDialogueWidget()
{
	CurDialogueWidget->SetUpEmpty();
	CurDialogueWidget->SetMiddleEmpty();
	CurDialogueWidget->SetDownEmpty();

	CurDialogueWidget->RemoveFromParent();
	CurDialogueWidget = nullptr;
}

void ADialogueManager::ChangeCurDialogueWidgetNormalText()
{
	check(CurDialogueWidget);

	if (NormalRow->FirstText.IsEmptyOrWhitespace() && NormalRow->SecondText.IsEmptyOrWhitespace()) {
		//둘다 비어있으면 넘김
		return;
	}
	else if (NormalRow->FirstText.IsEmptyOrWhitespace() || NormalRow->SecondText.IsEmptyOrWhitespace()) {
		//둘중 하나만 비어있으면 MiddleText만

		if (NormalRow->FirstText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(NormalRow->SecondText);
		else if (NormalRow->SecondText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(NormalRow->FirstText);

		CurDialogueWidget->SetUpEmpty();
		CurDialogueWidget->SetDownEmpty();
	}
	else {
		CurDialogueWidget->SetUpText(NormalRow->FirstText);
		CurDialogueWidget->SetDownText(NormalRow->SecondText);

		CurDialogueWidget->SetMiddleEmpty();
	}

}

void ADialogueManager::ChangeCurDialogueWidgetChoiceText()
{
	check(CurDialogueWidget);

	if (ChoiceRow->FirstText.IsEmptyOrWhitespace() && ChoiceRow->SecondText.IsEmptyOrWhitespace()) {
		//둘다 비어있으면 넘김
		return;
	}
	else if (ChoiceRow->FirstText.IsEmptyOrWhitespace() || ChoiceRow->SecondText.IsEmptyOrWhitespace()) {
		//둘중 하나만 비어있으면 MiddleText만

		if (ChoiceRow->FirstText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(ChoiceRow->SecondText);
		else if (ChoiceRow->SecondText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(ChoiceRow->FirstText);

		CurDialogueWidget->SetUpEmpty();
		CurDialogueWidget->SetDownEmpty();
	}
	else {
		CurDialogueWidget->SetUpText(ChoiceRow->FirstText);
		CurDialogueWidget->SetDownText(ChoiceRow->SecondText);

		CurDialogueWidget->SetMiddleEmpty();
	}
}

void ADialogueManager::ChangeCurDialogueWidgetChoice()
{
	CurDialogueWidget;
	ChoiceRow;

	int NumCount = 0;

	if (!ChoiceRow->ChoiceTextAnswer1.IsNone()) NumCount++;
	if (!ChoiceRow->ChoiceTextAnswer2.IsNone()) NumCount++;
	if (!ChoiceRow->ChoiceTextAnswer3.IsNone()) NumCount++;
	if (!ChoiceRow->ChoiceTextAnswer4.IsNone()) NumCount++;

	if (NumCount == 2) {
		Cast<UChoiceDialogueWidget>(CurDialogueWidget)->SetTwoChoice();
	}
	else if (NumCount == 3) {
		Cast<UChoiceDialogueWidget>(CurDialogueWidget)->SetThreeChoice();
	}
	else if(NumCount == 4){
		Cast<UChoiceDialogueWidget>(CurDialogueWidget)->SetFourChoice();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NO CHOICENUM"));
		return;
	}

}

void ADialogueManager::EndDialogue()
{
	if(CurDialogueWidget) RemoveCurDialogueWidget();
	
	ProjectPlayerController->AllowLookMove();
	SetUseIdalogue(false);
}

void ADialogueManager::NextNormalDialogue() 
{
	StartDialogue(NextID, UIType);
}

void ADialogueManager::NextChoiceDialogue()
{

}

bool ADialogueManager::GetUseDialogue()
{
	return bDialogue;
}

void ADialogueManager::SetUseIdalogue(bool _value)
{
	bDialogue = _value;
}

