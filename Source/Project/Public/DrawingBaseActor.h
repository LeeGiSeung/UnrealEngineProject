// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "DrawingBaseActor.generated.h"

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMeshComponent* RenderMesh;

};
