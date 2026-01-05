// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPlayerController.generated.h"

class APawn;
class ACharacter;
class UCameraComponent;
class APostProcessVolume;
class ACameraActor;
class UBaseAnimInstance;

/**
 *
 */
UCLASS()
class PROJECT_API AProjectPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	//Base
	AProjectPlayerController();
	~AProjectPlayerController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaTime) override;
	//~Base

	void SpecialCameraUse();
	void CameraGrayTrans();
	void CameraColorTrans();
	void ReturnToPlayerCamera();
	void SpecialCameraSetting();

	//BluePrint Function
	UFUNCTION(BlueprintCallable, Category = "Timer")
	void StartRealTimeTimer();

	UFUNCTION(BlueprintCallable, Category = "SpecialAttack")
	void StartSpecialAttack();

	UFUNCTION(BlueprintCallable, Category = "ScrollZoom")
	void ScrollZomm_Down(float ActionValue);

	UFUNCTION(BlueprintCallable, Category = "ScrollZoom")
	void ScrollZomm_Up(float ActionValue);
	//~BluePrint Function
	

	//BluePrint Uproperty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool IsBlackWhite = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float TimeDilation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float TimeDuration;
	
	UPROPERTY(EditAnywhere, Category = "SpecialCamera")
	UCameraComponent* FaceCameraAnchor;

	UPROPERTY(EditAnywhere, Category = "SpecialCamera")
	ACameraActor* FaceCameraActor;
	//~BluePrint Uproperty
	UPROPERTY(BlueprintReadOnly, Category="CameraDistance")
	float CameraDistance = 360.f;
	float CurCameraDistance = 0.f;

private:

	APawn* PPawn = NULL;
	ACharacter* PCharacter = NULL;
	UCameraComponent* PCamera = NULL;

	APostProcessVolume* PostProcessVolume;

	UBaseAnimInstance* ABP_Player;

	float StartTime;

};
