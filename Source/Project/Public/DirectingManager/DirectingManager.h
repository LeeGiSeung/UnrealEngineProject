// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequence.h"
#include "RowFolder/EventRow.h"

#include "DirectingManager.generated.h"

UCLASS()
class PROJECT_API ADirectingManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADirectingManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PlayEvent(FName DirectingKey);
	void PlayLevelSequence(ULevelSequence* Sequence);

	UPROPERTY(EditAnywhere, Category = "LevelSequence")
	UDataTable* EventTable;

	const FEventRow* Row;
	
};
