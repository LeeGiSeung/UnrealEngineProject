// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DrawingActorManager.h"
#include "DrawingBaseActor.h"

ADrawingActorManager::ADrawingActorManager()
{
}

void ADrawingActorManager::AddDrawingActor(ADrawingBaseActor* _actor)
{
	aDrawingActorList.Add(_actor);
	UE_LOG(LogTemp, Warning, TEXT("SIZE : "), aDrawingActorList.Num());
}

void ADrawingActorManager::DeleteDrawingActor(ADrawingBaseActor* _actor)
{
	aDrawingActorList.Remove(_actor);
}

ADrawingBaseActor* ADrawingActorManager::GetDrawingActor(ADrawingBaseActor* _actor)
{
	ADrawingBaseActor* result = nullptr;

	for (ADrawingBaseActor* actor : aDrawingActorList) {
		if (actor == _actor) {
			result = actor;
			break;
		}
	}

	return result;
}


