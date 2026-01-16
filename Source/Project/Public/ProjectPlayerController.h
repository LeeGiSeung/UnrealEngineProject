// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Colorenum/Colorenum.h"

#include "ProjectPlayerController.generated.h"

class APawn;
class ACharacter;
class AProjectCharacter;
class UCameraComponent;
class APostProcessVolume;
class ACameraActor;
class UBaseAnimInstance;
class ADrawingBaseActor;
class UFWidget;
class ADrawing_Decal_Actor;
class ADecalActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionTriggered);

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

	UFUNCTION(BlueprintCallable, Category = "Drawing")
	void SpawnDecalActor(TArray<FVector2D> _DrawPosition , EColor CurChoiceColor);

	UFUNCTION(BlueprintCallable, Category = "Drawing_Object")
	void DrawingObject_UseAbility();

	void RegisterDrawingActor(ADrawingBaseActor* _ADrawingBaseActor);
	void UnregisterDrawingActor(ADrawingBaseActor* _ADrawingBaseActor);

	void RegisterDrawingDecar(ADrawing_Decal_Actor* _ADrawingBaseDecar);
	void UnregisterDrawingDecar(ADrawing_Decal_Actor* _ADrawingBaseDecar);

	//~BluePrint Function
	

	//BluePrint Uproperty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawingObject")
	TSubclassOf<UFWidget> InteractWidgetClass;

	UPROPERTY()
	UFWidget* InteractWidget;

	UPROPERTY()
	UStaticMesh* CubeMesh;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnActionTriggered OnActionTriggered;

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

	UPROPERTY(BlueprintReadWrite, Category = "Drawing")
	TArray<FVector2D> DrawPosition;

	UPROPERTY(BlueprintReadOnly, Category = "DrawingObject")
	bool bFindObject = false;

	UPROPERTY(BlueprintReadOnly, Category = "DrawingObject")
	ADrawingBaseActor* DrawingActor;

	//DrawingObject Hit Postion
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DrawingObject")
	TMap<EColor, TSubclassOf<AActor>> SpawnActorMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DrawingObject")
	TMap<EColor, UMaterialInterface*> DecalMaterialMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DrawingObject")
	TMap<UMaterialInterface*, TSubclassOf<AActor>> DecarDrawingObjectMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DrawingObject")
	EColor DrawingColor;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCenterCurSor")
	//UMaterialInstanceDynamic* CenterCursorMaterial;
	
	FHitResult Hit; //공유되는 hit

	ADecalActor* Decal;
	//DrawingObject Hit Postion
	
	float CheckInterval = 0.0f;
	float CheckAccTime = 0.f;
	TArray<TWeakObjectPtr<ADrawingBaseActor>> TrackedActors;

	TArray<TWeakObjectPtr<ADrawing_Decal_Actor>> TrackedDecalActors;

	float DefaultWalkSpeed = 600.f;
	float DashSpeed = 1100.f;
	float ViewDistance = 10000.f;
	bool bIsDashing = false;

	FHitResult GetHit();

	void SpawnCubeAtHit();
	void SpawnDecalAtHit();

private:

	APawn* PPawn = NULL;
	ACharacter* PCharacter = NULL;
	AProjectCharacter* ProjectChar = NULL;
	UCameraComponent* PCamera = NULL;

	APostProcessVolume* PostProcessVolume;

	UBaseAnimInstance* ABP_Player;

	float StartTime;

};
