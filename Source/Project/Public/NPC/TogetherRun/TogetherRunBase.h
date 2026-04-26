// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TogetherRunBase.generated.h"

class AProjectCharacter;
class UTogetherRunAnimInstance;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerActorReference, AProjectCharacter*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnfGroundSpeedFromPlayer, float, FVector);

UCLASS()
class PROJECT_API ATogetherRunBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATogetherRunBase();

	FOnfGroundSpeedFromPlayer OnfGroundSpeedFromPlayer;

	FOnPlayerActorReference FReferenceProjectPlayer;

	void SetFrontActorReference(AProjectCharacter* value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetTogetherActorTurn();

	void BasicSetting();

public:	
	// Called every frame
	

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite, Category = "MovementData")
	float fGroundSpeed;

	UPROPERTY(BlueprintReadWrite, Category = "MovementData")
	bool bShouldMove;

	UPROPERTY()
	AProjectCharacter* Player;

	UPROPERTY()
	UTogetherRunAnimInstance* TogetherRunAnimInstance;

	UPROPERTY()
	FVector PlayerRightHandLocation;

	UPROPERTY()
	ATogetherRunBase* ChainTogetherActor;

	void SetTogetherActorSpeed(AActor* FrontActor ,float value, FVector HandLocation);

	FName TogetherNPCLeftHandName = "hand_l_Socket";

	FName TogetherNPCRIghtHandName = "hand_r_Socket";
	

	FVector NPCHaneSocketNameLocation;


	AActor* GetTogetherBaseLeaderObject();
	
	UPROPERTY()
	int chainIndex;

	void SetChainIndex(int value);
	int GetChainIndex();

};
