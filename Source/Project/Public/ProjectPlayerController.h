// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectPlayerController.generated.h"

class APawn;
class ACharacter;
class AProjectCharacter;
class UCameraComponent;
class APostProcessVolume;
class ACameraActor;
class UBaseAnimInstance;
class UBaseUserWidget;

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

	UFUNCTION(BlueprintCallable, Category = "Dash")
	void DashStart();

	UFUNCTION(BlueprintCallable, Category = "Dash")
	void DashEnd();

	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void DrawingStart();

	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void DrawingEnd();
	//~BluePrint Function
	

	//BluePrint Uproperty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool IsBlackWhite = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool IsDash = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float TimeDilation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	float TimeDuration;
	
	UPROPERTY(EditAnywhere, Category = "SpecialCamera")
	USceneComponent* FaceCameraAnchor;

	UPROPERTY(EditAnywhere, Category = "SpecialCamera")
	ACameraActor* FaceCameraActor;
	//~BluePrint Uproperty
	UPROPERTY(BlueprintReadOnly, Category="CameraDistance")
	float CameraDistance = 360.f;
	UPROPERTY(BlueprintReadOnly, Category = "CameraDistance")
	float CurCameraDistance = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Drawing")
	float fCanvasSizeWidth = 1920;

	UPROPERTY(BlueprintReadOnly, Category = "Drawing")
	float fCanvasSizeHeight = 1080;

	UPROPERTY(BlueprintReadOnly, Category = "Drawing")
	bool bIsDrawing;

	float DefaultWalkSpeed = 600.f;
	float DashSpeed = 1100.f;
	bool bIsDashing = false;

private:

	APawn* PPawn = NULL;
	ACharacter* PCharacter = NULL;
	AProjectCharacter* ProjectChar = NULL;
	UCameraComponent* PCamera = NULL;

	APostProcessVolume* PostProcessVolume;

	UBaseAnimInstance* ABP_Player;

	float StartTime;

};
