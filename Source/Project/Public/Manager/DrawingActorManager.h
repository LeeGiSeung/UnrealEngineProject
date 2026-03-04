// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "DrawingActorManager.generated.h"

class ADrawingBaseActor;
class ADecalActor;
/**
 * 
 */
UCLASS()
class PROJECT_API ADrawingActorManager : public ABaseActor
{
	GENERATED_BODY()
	
public:
	ADrawingActorManager();
	TArray<TWeakObjectPtr<ADrawingBaseActor>> aDrawingActorList;

	void AddDrawingActor(ADrawingBaseActor* _actor);
	void DeleteDrawingActor(ADrawingBaseActor* _actor);
	void ChangeRandomActorDecalBlack();
	void ChangeRandomActorOriginalColor();

	ADrawingBaseActor* GetDrawingActor(ADrawingBaseActor* _actor);
	UPROPERTY(BlueprintReadWrite, Category = "BlackDecal")
	UMaterialInterface* BlackDecal;
};
