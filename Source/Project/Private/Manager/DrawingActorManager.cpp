// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DrawingActorManager.h"
#include "Engine/DecalActor.h"
#include "DrawingBaseActor.h"

ADrawingActorManager::ADrawingActorManager()
{
}

void ADrawingActorManager::AddDrawingActor(ADrawingBaseActor* _actor)
{
	aDrawingActorList.Add(_actor);
}

void ADrawingActorManager::DeleteDrawingActor(ADrawingBaseActor* _actor)
{
	aDrawingActorList.Remove(_actor);
}

void ADrawingActorManager::ChangeRandomActorDecalBlack()
{
	for (const TWeakObjectPtr<ADrawingBaseActor>& WeakActor : aDrawingActorList) {
		if (WeakActor.IsValid() && WeakActor->GetRandom()) {
			ADecalActor* decal = WeakActor->Decal;
			
			decal->SetDecalMaterial(BlackDecal);
		}
	}
}

void ADrawingActorManager::ChangeRandomActorOriginalColor()
{
	for (const TWeakObjectPtr<ADrawingBaseActor>& WeakActor : aDrawingActorList) {
		if (WeakActor.IsValid() && WeakActor->GetRandom()) {
			ADecalActor* decal = WeakActor->Decal;

			decal->SetDecalMaterial(WeakActor->GetDecalMaterial());
		}
	}
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


