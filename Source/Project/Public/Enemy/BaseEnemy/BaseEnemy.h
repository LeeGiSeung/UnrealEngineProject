// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "City/MapWidget/Marker/MapViewer/MapMarkerInterface/MapMarkerInterface.h"

#include "BaseEnemy.generated.h"

class AAIController;
class UBlackboardComponent;
class UMinimapWorldSystem;
class UEnemyManager;
class UEnemyMarker;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoved, AActor*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTurnd, AActor*);

UCLASS()
class PROJECT_API ABaseEnemy : public ACharacter, public IMapMarkerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AAIController* AIController;
	UBlackboardComponent* BBoard;
	APawn* PlayerPawn;

	virtual void IncreaseHP(int value);
	virtual void DecreaseHP(int value);

	virtual void EnemyDie();

	UPROPERTY()
	UMinimapWorldSystem* MinimapWorld;
	
	UPROPERTY()
	UEnemyManager* EnemyManager;

	UPROPERTY()
	UPointMarker* EnemyMarker;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "MarkerClass")
	TSubclassOf<UEnemyMarker> EnemyMarkerClass;

	virtual TSubclassOf<UUserWidget> GetMarkerClass() const override;
	virtual UPointMarker* GetPointMarker() const override;
	virtual void SetPointMarker(UPointMarker* Value) override;

	FVector prevEnemyLocation;
	FRotator prevEnemyRotation;

	FOnMoved OnMoved;
	FOnTurnd OnTurnd;

protected:
	uint32 EnemyHP;

};
