// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DrawingBaseActor.h"
#include "Drawing_Tree_Actor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API ADrawing_Tree_Actor : public ADrawingBaseActor
{
	GENERATED_BODY()

public:
	ADrawing_Tree_Actor();
	virtual void UseAbility() override;


public:
	virtual void Tick(float DeltaTime) override;

	void GrowTree();

	UPROPERTY(BlueprintReadWrite, Category = "Drawing_Tree")
	float GrowSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "Drawing_Tree")
	float DuarationTime = 10.f;

	UPROPERTY(BlueprintReadWrite, Category = "Drawing_Tree")
	float GrowTime = 5.f;

	float CurTime = 0.f;

	bool isGrow = false;

protected:
	virtual void BeginPlay() override;

};
