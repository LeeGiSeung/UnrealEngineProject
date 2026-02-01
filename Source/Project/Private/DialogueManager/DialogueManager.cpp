// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueManager/DialogueManager.h"
#include "DialogueType/DialogueType.h"
#include "DirectingManager/DirectingManager.h"

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

	//현재 보여줘야 하는 컷신을 정해줌
	ShowCurDialogue();
}

//현재 적용돼있는 ID에 해당하는 Dialogue를 보여줌
void ADialogueManager::ShowCurDialogue()
{

	//연출도 해야함
	//DirectingManager에 연출 row 보내서 연출 시작
}

