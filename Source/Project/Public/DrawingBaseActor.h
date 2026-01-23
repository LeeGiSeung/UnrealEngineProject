// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "DrawingBaseActor.generated.h"

class UBoxComponent;
class ADecalActor;
/**
 * 
 */
UCLASS()
class PROJECT_API ADrawingBaseActor : public ABaseActor
{
	GENERATED_BODY()
	
public:

	ADrawingBaseActor();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void UseAbility();

	virtual void SetHighlight(bool bEnable);

	void SetDecalActor(ADecalActor* Decal);
	ADecalActor* GetDecalActor();
	ADecalActor* Decal;

	void SetInUse();

	bool bInUse = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UMeshComponent* RenderMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RenderMesh;
};
