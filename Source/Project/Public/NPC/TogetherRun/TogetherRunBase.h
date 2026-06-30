// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "City/MapWidget/Marker/MapViewer/MapMarkerInterface/MapMarkerInterface.h"
#include "TogetherRunBase.generated.h"

class AProjectCharacter;
class UTogetherRunAnimInstance;
class ATogetherRunBase;
class UTogetherActorMarker;
class UTogetherManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoved,AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTurnd,AActor*);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerActorReference, AProjectCharacter*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnfGroundSpeedFromPlayer, float, FVector);



UCLASS()
class PROJECT_API ATogetherRunBase : public ACharacter, public IMapMarkerInterface
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

public:
	FVector prevTogetherLocation;
	FRotator prevTogetherRotation;

public:
	FOnMoved OnMoved;
	FOnTurnd OnTurnd;

	UTogetherManager* TogetherManager;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MarkerClass")
	TSubclassOf<UTogetherActorMarker> TogetherActorMarkerClass;

	UPointMarker* TogetherActorMarker;

	virtual TSubclassOf<UUserWidget> GetMarkerClass() const override;
	virtual UPointMarker* GetPointMarker() const override;
	virtual void SetPointMarker(UPointMarker* Value) override;

private:


};
