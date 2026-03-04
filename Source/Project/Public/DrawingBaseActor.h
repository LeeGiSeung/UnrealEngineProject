// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "DrawingBaseActor.generated.h"

class UBoxComponent;
class ADecalActor;
class UMaterialInstance;
class ADrawingActorManager;

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

	//####################Decal °ü¸®
	void SetDecalActor(ADecalActor* Decal);
	UMaterialInterface* OriginalDecalMaterial = nullptr;

	void SetDcalMaterial(UMaterialInterface* Material) {
		OriginalDecalMaterial = Material;
	}

	UMaterialInterface* GetDecalMaterial() {
		return OriginalDecalMaterial;
	}

	ADecalActor* GetDecalActor();
	ADecalActor* Decal;
	//###################

	void SetInUse();

	bool bInUse = false;

	void SetRandom(bool _b) { bIsRandom = _b; }
	bool GetRandom() { return bIsRandom; }
	bool bIsRandom = false;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//UMeshComponent* RenderMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* RenderMesh;

	ADrawingActorManager* DrawingManager;
};
