// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TogetherRunBase.generated.h"

class AProjectCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerActorReference, AProjectCharacter*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnfGroundSpeedFromPlayer, float);

UCLASS()
class PROJECT_API ATogetherRunBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATogetherRunBase();

	FOnfGroundSpeedFromPlayer OnfGroundSpeedFromPlayer;

	FOnPlayerActorReference FReferenceProjectPlayer;

	void SetProjectPlayerReference(AProjectCharacter* Player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, Category = "GroundSpeed")
	float fGroundSpeed;

	UPROPERTY()
	AProjectCharacter* PlayerCharacter;

	void SetTogetherActorSpeed(float value);

};
