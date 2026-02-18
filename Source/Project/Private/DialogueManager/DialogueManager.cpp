// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager/DialogueManager.h"
#include "DialogueType/DialogueType.h"
#include "Engine/DataTable.h"

#include "EngineUtils.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

//#각 Widget들
#include "DialogueWidget/ChoiceDialogueWidget/ChoiceDialogueWidget.h"
#include "DialogueWidget/NormalDialogueWidget/NormalDialogueWidget.h"
#include "DialogueWidget/AutoDialogueWidget/AutoDialogueWidget.h"
#include "BaseUserWidget.h"
#include "Energy/EnergyWidget.h"
#include "DialogueWidget/BaseDialogueWidget.h"

//#SoundManager
#include "Manager/SoundManager.h"

ADialogueManager::ADialogueManager()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADialogueManager::BeginPlay()
{
	Super::BeginPlay();

	ProjectPlayerController = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());

	SoundManager = GetWorld()->GetGameInstance()->GetSubsystem<USoundManager>();
		
	if (!SoundManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO SOUNDMANAGER"));
	}

	for (TActorIterator<ADirectingManager> It(GetWorld()); It; ++It)
	{
		DirectingManager = *It;
		break;
	}

	if (!DirectingManager)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO DIRECTINGMANAGER"));
	}

	NormalWidget = CreateWidget<UNormalDialogueWidget>(
		GetWorld()->GetFirstPlayerController(),
		DialogueWidgetMap[EDialogueUIType::Normal]
	);

	ChoiceWidget = CreateWidget<UChoiceDialogueWidget>(
		GetWorld()->GetFirstPlayerController(),
		DialogueWidgetMap[EDialogueUIType::Choice]
	);

	AutoWidget = CreateWidget<UAutoDialogueWidget>(
		GetWorld()->GetFirstPlayerController(),
		DialogueWidgetMap[EDialogueUIType::Auto]
	);

	NormalWidget->AddToViewport();
	ChoiceWidget->AddToViewport();
	AutoWidget->AddToViewport();

	NormalWidget->SetVisibility(ESlateVisibility::Collapsed);
	ChoiceWidget->SetVisibility(ESlateVisibility::Collapsed);
	AutoWidget->SetVisibility(ESlateVisibility::Collapsed);

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

	SaveAndRemoveAllWidgets();
	
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

	ResetCurDialogueWidget();
	
	switch (UIType) {
	case EDialogueUIType::Normal:
		
		NormalRow = DialogueNormalTable->FindRow<FDialogueRow>(ID, TEXT("DialogueText"));
		
		if (!NormalRow) {
			EndDialogue();
			UE_LOG(LogTemp, Warning, TEXT("No NormalRow"));
			return;
		}

		NextID = NormalRow->NextID;

		if (!NormalRow->Speaker.IsNone()) {
			SoundManager->PlayDialogueSound(NormalRow->Speaker, EDialogueUIType::Normal);
		}

		EventKey = NormalRow->DirectingKey;

		break;
		//선택지 넘길 수 있게 추가해야함

	case EDialogueUIType::Choice:
		ChoiceRow = DialogueChoiceTable->FindRow<FChoiceDialogueRow>(ID, TEXT("No Choice"));

		if (!ChoiceRow) {
			EndDialogue();
			UE_LOG(LogTemp, Warning, TEXT("No ChoiceRow"));
			return;
		}

		PlayerChoiceNumberCheck();

		if (!ChoiceRow->Speaker.IsNone()) {
			SoundManager->PlayDialogueSound(ChoiceRow->Speaker, EDialogueUIType::Choice);
		}

		EventKey = ChoiceRow->DirectingKey;

		break;

	case EDialogueUIType::Auto:

		AutoRow = DialogueAutoTable->FindRow<FAutoDialogueRow>(ID, TEXT("No Auto"));

		if (!AutoRow) {
			EndDialogue();
			UE_LOG(LogTemp, Warning, TEXT("No AutoRow"));
			return;
		}

		NextID = AutoRow->NextID;

		if (!AutoRow->Speaker.IsNone()) {
			SoundManager->PlayDialogueSound(AutoRow->Speaker, EDialogueUIType::Auto);
		}

		EventKey = AutoRow->DirectingKey;

		break;


	case EDialogueUIType::End:
		EndDialogue();
		return;
	}

	if (!EventKey.IsNone()) {
		DirectingManager->PlayEvent(EventKey);
	}

	CurUIType = UIType;

	//CurDialogueWidget = CreateWidget<UBaseDialogueWidget>(
	//	GetWorld()->GetFirstPlayerController(),
	//	DialogueWidgetMap[UIType]);


	switch (UIType)
	{
	case EDialogueUIType::Normal:
		
		CurDialogueWidget = NormalWidget;
		ChangeCurDialogueWidgetNormalText();

		UIType = NormalRow->UIType;
			
		break;

	case EDialogueUIType::Choice:
		ChoiceWidget->SetChoiceBaseSetting();
		CurDialogueWidget = ChoiceWidget;
		
		ChangeCurDialogueWidgetChoiceText();
		ChangeCurDialogueWidgetChoice();

		UIType = ChoiceRow->UIType;
		break;

	case EDialogueUIType::Auto:
		
		AutoWidget->SetAutoBaseSetting();

		ChangeCurDialogueWidgetAutoText();

		CurDialogueWidget = AutoWidget;
		UIType = AutoRow->UIType;
		break;
	}

	//WidgetAddViewPort();
	CurDialogueWidget->SetVisibility(ESlateVisibility::Visible);

	
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

void ADialogueManager::ResetCurDialogueWidget()
{
	if (!CurDialogueWidget) return;
	CurDialogueWidget->SetUpEmpty();
	CurDialogueWidget->SetMiddleEmpty();
	CurDialogueWidget->SetDownEmpty();

	CurDialogueWidget->SetVisibility(ESlateVisibility::Collapsed);

	//CurDialogueWidget->RemoveFromParent();
	//CurDialogueWidget = nullptr;
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

void ADialogueManager::ChangeCurDialogueWidgetAutoText()
{
	if (AutoRow->FirstText.IsEmptyOrWhitespace() && AutoRow->SecondText.IsEmptyOrWhitespace()) {
		//둘다 비어있으면 넘김
		return;
	}
	else if (AutoRow->FirstText.IsEmptyOrWhitespace() || AutoRow->SecondText.IsEmptyOrWhitespace()) {
		//둘중 하나만 비어있으면 MiddleText만

		if (AutoRow->FirstText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(AutoRow->SecondText);
		else if (AutoRow->SecondText.IsEmptyOrWhitespace()) CurDialogueWidget->SetMiddleText(AutoRow->FirstText);

		CurDialogueWidget->SetUpEmpty();
		CurDialogueWidget->SetDownEmpty();
	}
	else {
		CurDialogueWidget->SetUpText(AutoRow->FirstText);
		CurDialogueWidget->SetDownText(AutoRow->SecondText);

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
	if(CurDialogueWidget) ResetCurDialogueWidget();
	
	ProjectPlayerController->AllowLookMove();
	ProjectPlayerController->ResetDialogueActor();
	ShowAllWidget();
	SetUseIdalogue(false);
}

void ADialogueManager::SaveAndRemoveAllWidgets()
{
	if (CursorWidget && EnergyWidget) {
		CursorWidget->SetVisibility(ESlateVisibility::Collapsed);
		EnergyWidget->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NO WIDGET"));
		return;
	}
	//StoredWidgets.Empty();

	//TArray<UUserWidget*> FoundWidgets;

	//UWidgetBlueprintLibrary::GetAllWidgetsOfClass(
	//	GetWorld(),
	//	FoundWidgets,
	//	UUserWidget::StaticClass(),
	//	false   // TopLevelOnly (보통 false 추천)
	//);

	//for (UUserWidget* Widget : FoundWidgets)
	//{
	//	if (!Widget) continue;

	//	StoredWidgets.Add(Widget);
	//	Widget->RemoveFromParent();
	//}

	//기존 Widget Middle, EnerhyBar 등 따로 참조한다음에 이걸 끊어야함

}

void ADialogueManager::ShowAllWidget()
{
	if (CursorWidget && EnergyWidget) {
		CursorWidget->SetVisibility(ESlateVisibility::Visible);
		EnergyWidget->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("NO WIDGET"));
		return;
	}

	//if (StoredWidgets.Num() == 0) return;

	//for (UUserWidget* Widget : StoredWidgets)
	//{
	//	if (!Widget) continue;

	//	if (!Widget->IsInViewport())
	//	{
	//		Widget->AddToViewport();
	//	}
	//}

	//StoredWidgets.Empty();
	//기존 Widget Middle, EnerhyBar 등 따로 참조한걸 다시 visibitlity 해줘야함

}

void ADialogueManager::NextNormalDialogue() 
{
	if (!DirectingManager || !SoundManager) return;

	DirectingManager->SetLevelSequencePlay(true);
	SoundManager->StopDialogueSound();

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

void ADialogueManager::SetCursorWidget(UBaseUserWidget* widget) {
	if (CursorWidget) return;
	CursorWidget = widget;
}

void ADialogueManager::SetEnergyWidget(UEnergyWidget* widget) {
	if (EnergyWidget) return;
	EnergyWidget = widget;
}
