// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "BP_CablePouch.generated.h"

class UStaticMeshComponent;
class AProjectPlayerController;
class AProjectCharacter;

/**
 * 
 */
UCLASS()
class PROJECT_API ABP_CablePouch : public ABaseActor
{
	GENERATED_BODY()
public:

	ABP_CablePouch();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void UsePouch(FVector2D _Value);

	FVector fInitalPosition;

	float XPower;
	float YPower;
	float ZPower;

private:
	AProjectPlayerController* PlayerController;
	AProjectCharacter* ProjectChacter;

};
