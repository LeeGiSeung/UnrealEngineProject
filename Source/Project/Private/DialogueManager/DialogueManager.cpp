// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager/DialogueManager.h"
#include "DialogueType/DialogueType.h"
#include "Engine/DataTable.h"

#include "DirectingManager/DirectingManager.h"

//#각 Widget들
#include "DialogueWidget/NormalDialogueWidget/NormalDialogueWidget.h"
#include "DialogueWidget/BaseDialogueWidget.h"

ADialogueManager::ADialogueManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADialogueManager::BeginPlay()
{
	Super::BeginPlay();
	
}

//외부 Actor로 부터 상호작용해 컷신 시작하기
void ADialogueManager::StartDialogue(FName _ID)
{
	//여기서 csv에 접근해서 ID, eventkey 등 여러개 가져와야함

	ID = _ID;
	SetUseIdalogue(true);
	//현재 보여줘야 하는 컷신을 정해줌
	ShowCurDialogue();
}

//현재 적용돼있는 ID에 해당하는 Dialogue를 보여줌
void ADialogueManager::ShowCurDialogue()
{
	if (!DialogueTable) {
		UE_LOG(LogTemp, Warning, TEXT("No DataTable"));
		return;
	}
	
	if (DialogueWidgetMap.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("No Widget Map"));
		return;
	}

	Row = DialogueTable->FindRow<FDialogueRow>(ID, TEXT("DialogueText"));

	if (!Row) {
		UE_LOG(LogTemp, Warning, TEXT("No Row"));
		return;
	}

	NextID = Row->NextID;

	if (Row->UIType == EDialogueUIType::End) {
		EndDialogue();
		return;
	}

	if (CurDialogueWidget) { //현재 사용되고있는 Widget이 있으면 그 위젯을 지우고 새로운 위젯
		RemoveCurDialogueWidget();
	}

	CurDialogueWidget = CreateWidget<UBaseDialogueWidget>(GetWorld()->GetFirstPlayerController(), DialogueWidgetMap[Row->UIType]);
	
	if (CurDialogueWidget) {
		WidgetAddViewPort();
		ChangeCurDialogueWidgetText();
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("NO WIDGET"));
		return;
	}
	//연출도 해야함 DirectingManager에 연출 row 보내서 연출 시작
}


















void ADialogueManager::WidgetAddViewPort()
{
	CurDialogueWidget->AddToViewport();
}

void ADialogueManager::RemoveCurDialogueWidget()
{
	CurDialogueWidget->RemoveFromParent();
	CurDialogueWidget = nullptr;
}

void ADialogueManager::ChangeCurDialogueWidgetText()
{
	check(CurDialogueWidget);

	//Row의 FirstText, SecondText를 통해 Text 배열 판단

	if (Row->FirstText.IsEmptyOrWhitespace() && Row->SecondText.IsEmptyOrWhitespace()) {
		//둘다 비어있으면 넘김
		return;
	}
	else if (Row->FirstText.IsEmptyOrWhitespace() || Row->SecondText.IsEmptyOrWhitespace()) {
		//둘중 하나만 비어있으면 MiddleText만

		if (Row->FirstText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(Row->SecondText);
		else if (Row->SecondText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(Row->FirstText);

		CurDialogueWidget->SetUpEmpty();
		CurDialogueWidget->SetDownEmpty();
	}
	else {
		CurDialogueWidget->SetUpText(Row->FirstText);
		CurDialogueWidget->SetDownText(Row->SecondText);

		CurDialogueWidget->SetMiddleEmpty();

	}

}

void ADialogueManager::EndDialogue()
{
	RemoveCurDialogueWidget();
}

void ADialogueManager::NextDialogue() 
{
	StartDialogue(NextID);
}

bool ADialogueManager::GetUseDialogue()
{
	return bDialogue;
}

void ADialogueManager::SetUseIdalogue(bool _value)
{
	bDialogue = _value;
}

