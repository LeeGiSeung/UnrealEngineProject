// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DrawingActorManager.h"
#include "DrawingBaseActor.h"

ADrawingActorManager::ADrawingActorManager()
{
}

void ADrawingActorManager::AddDrawingActor(ADrawingBaseActor* _actor)
{
	aDrawingActorList.Add(_actor);
	UE_LOG(LogTemp, Warning, TEXT("add size : %d"), aDrawingActorList.Num());
}

void ADrawingActorManager::DeleteDrawingActor(ADrawingBaseActor* _actor)
{
	aDrawingActorList.Remove(_actor);
	UE_LOG(LogTemp, Warning, TEXT("del size %d"), aDrawingActorList.Num());

}

ADrawingBaseActor* ADrawingActorManager::GetDrawingActor(ADrawingBaseActor* _actor)
{
	ADrawingBaseActor* result = nullptr;

	for (const TWeakObjectPtr<ADrawingBaseActor>& WeakActor : aDrawingActorList)
	{
		if (WeakActor.IsValid() && WeakActor.Get() == _actor)
		{
			result = WeakActor.Get();
			break;
		}
	}

	return result;
}


