// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Colorenum/Colorenum.h"

//Dialouge
#include "DialogueBaseActor/DialogueBaseActor.h"

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
class UEnergyWidget;
class ADrawingActorManager;
class ABP_CablePouch;
class ADialogueManager;
class UBaseUserWidget;

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
	virtual void BeginPlay() override;
	//~Base

	void SpecialCameraUse();
	void CameraGrayTrans();
	void CameraColorTrans();
	void ReturnToPlayerCamera();
	void AllowLookMove();
	void IgnoreLookMove();
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

	void SpawnRandomActor();

	UFUNCTION(BlueprintCallable, Category = "Drawing_Object")
	void DrawingObject_UseAbility();

	UFUNCTION(BlueprintCallable, Category = "CablePouch")
	void UseCable();

	void UnUseCable();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetCursorWidget(UBaseUserWidget* widget);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEnergyWidget(UEnergyWidget* widget);


	void RegisterDrawingActor(ADrawingBaseActor* _ADrawingBaseActor);
	void UnregisterDrawingActor(ADrawingBaseActor* _ADrawingBaseActor);

	void RegisterDrawingDecar(ADrawing_Decal_Actor* _ADrawingBaseDecar);
	void UnregisterDrawingDecar(ADrawing_Decal_Actor* _ADrawingBaseDecar);

	bool CheckDrawingEnergyIsEnough(float _Scale);
	void UpdateEnergy(float _Energy);

	void SetEnergyPercentGage();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartDialogue();
	//~BluePrint Function
	

	//BluePrint Uproperty

	UPROPERTY(EditAnywhere, Category = "UI")
	TObjectPtr<UBaseUserWidget> CursorWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TObjectPtr<UEnergyWidget> EnergyWidget;

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

	UPROPERTY(BlueprintReadWrite, Category = "DrawingEnergyWidget")
	UEnergyWidget* DrawingEnergyWidget;

	UPROPERTY(BlueprintReadWrite, Category = "DrawingEnergyWidget")
	float fDrawingEnergy;
	
	float fDefaultDrawingEnergy;
	float fEnergyCharge = 0.1;

	bool bSpawnRandom = false;

	void SetSpawnRandom(bool Random) { bSpawnRandom = Random; }
	bool GetSpawnRandom() { return bSpawnRandom; }

	FHitResult Hit; //공유되는 hit

	FVector2D DrowSize;

	float ActorSpawnScaleX;
	float ActorSpawnScaleY;
	float ActorXSize = 0.f;
	//float ActorYSize = 0.f;
	//float ActorZSize = 0.f;
	FVector GetActorSpawnScale();
	void SetActorSpawnScale(float _ActorSpawnScaleX, float _ActorSpawnScaleY);

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

	ADrawingActorManager* DrawingManager;

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

//Cable Pouch
public:
	bool bUseCablePouch = false;

	void SetUseCablePouch(bool _value) { bUseCablePouch = _value; }
	bool GetUseCablePouch() { return bUseCablePouch; }

	UFUNCTION(BlueprintCallable, Category = "Cable Pouch")
	void UsePouchFly();

	ABP_CablePouch* CurUsePouch;

	ABP_CablePouch* GetUsingPouch() {
		ABP_CablePouch* Pouch = CurUsePouch;
		return Pouch;
	}

public:
	UAnimInstance* AnimInst;
	UBaseAnimInstance* MyABP;

	UFUNCTION(BlueprintCallable, Category = "CrouchBack")
	void StartCrouchBack();

//DialobueActor
public:
	void ResetDialogueActor();
	ADialogueBaseActor* diaActor;


private:
	ADialogueManager* DialogueManager;
};
