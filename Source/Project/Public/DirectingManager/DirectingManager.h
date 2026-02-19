// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelSequence.h"
#include "RowFolder/EventRow.h"
#include "LevelSequenceActor.h"
#include "DialogueType/DialogueType.h"

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

	void PlayEvent(FName DirectingKey, EDialogueUIType value);
	void PlayLevelSequence(ULevelSequence* Sequence);

	UPROPERTY(EditAnywhere, Category = "LevelSequence")
	UDataTable* EventTable;

	const FEventRow* Row;

	APawn* PlayerPawn;

	UPROPERTY(VisibleAnywhere, Category = "TagMap")
	TMap<FName, TWeakObjectPtr<AActor>> SequenceTagMap;

	ADirectingManager* GetDirectingManager(UWorld* World);

	void SetLevelSequencePlay(bool _value);
	bool GetLevelSequencePlay();

	UFUNCTION(BlueprintCallable, Category = "DirectingManager")
	void EndLevelSequence();

	void SetNextFrame(bool _value);
	bool GetNextFrame();

	UFUNCTION(BlueprintCallable, Category = "DirectingManager")
	void RegisterSequenceActor(FName Tag, AActor* Actor);

private:
	bool bLevelSequencePlay = false;
	bool bNextFrame = false;
	EDialogueUIType UIType;
	ULevelSequencePlayer* SequencePlayer;

	int32 LoopFrame;

};
