// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPlayerController.generated.h"

class APawn;
class ACharacter;
class UCameraComponent;
class APostProcessVolume;

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

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack")
	void StartSpecialAttack();

	void CameraGrayTrans();
	void CameraColorTrans();

	virtual void OnPossess(APawn* InPawn) override;

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

	APawn* PPawn = NULL;
	ACharacter* PCharacter = NULL;
	UCameraComponent* PCamera = NULL;
	
	APostProcessVolume* PostProcessVolume;

	float StartTime;

};
