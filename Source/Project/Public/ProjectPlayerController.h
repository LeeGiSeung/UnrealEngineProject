// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPlayerController.generated.h"

/**
 *
 */
UCLASS()
class PROJECT_API AProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartRealTimeTimer();

	void CameraGrayTrans();

	AProjectPlayerController();
	~AProjectPlayerController();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool IsBlackWhite = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")

	float TimeDilation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")

	float TimeDuration;

private:

	float StartTime;

	UPROPERTY()
	APawn* PPawn = NULL;
	UPROPERTY()
	AProjectCharacter* PCharacter = NULL;
	UPROPERTY()
	UCameraComponent* PCamera = NULL;

};
