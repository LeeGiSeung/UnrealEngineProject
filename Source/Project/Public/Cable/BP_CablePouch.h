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

	void UsePouch();
	void UnUsePouch();

	void FlyPlayer();

	void ResetPosition();

	FVector fInitalPosition;
	FRotator fInitalRotation;

	float XPower;
	float YPower;
	float ZPower;

	float MoveSpeed = 100.f;
	float LookSpeed = 100.f;

	bool bUsePouch = false;
	void SetbUsePouch(bool value) { bUsePouch = value; }
	bool GetbUsePouch() { return bUsePouch; }

private:
	AProjectPlayerController* PlayerController;
	AProjectCharacter* ProjectChacter;

};
