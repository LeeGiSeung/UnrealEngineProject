// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DirectingManager/DirectingManager.h"
#include "ProjectCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AProjectPlayerController;
class UBaseAnimInstance;
class UMinimapWidget;
class ATogetherRunBase;

struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FGetFGroundSpeedTo, float);

UCLASS(config=Game)
class AProjectCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SitAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlackWhiteAction;

public:
	AProjectCharacter();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpecialCamera")
	USceneComponent* FaceCameraAnchor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpecialCamera", meta = (AllowPrivateAccess = "true"))
	USceneComponent* CameraAnchor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpecialCamera")
	bool bIsDrawing;

	UPROPERTY()
	ADirectingManager* DirectingManager;

private:
	AProjectPlayerController* PlayerController;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	void Sit(const FInputActionValue& Value);

public: //내가 추가한거
	FVector2D CachedMoveInput;
	FVector2D CachedLookInput;

	void SetMoveInput(FVector2D value) { CachedMoveInput = value; }
	void SetLookInput(FVector2D value) { CachedLookInput = value; }

	FVector2D GetMoveInput() {
		FVector2D result = CachedMoveInput;
		CachedMoveInput = FVector2D::Zero();
		return result;
	}

	FVector2D GetLookInput() {
		FVector2D result = CachedLookInput;
		CachedLookInput = FVector2D::Zero();
		return result;
	}

	float PouchPush = 0.f;

	float GetPouchPush() { return PouchPush; }
	void ResetPouch() { PouchPush = 0.f; }

	void OnSpacePressed();

	UFUNCTION(BlueprintCallable)
	void StandUpTo();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	void FindTogetherRunActor();

	virtual void Tick(float DeltaTime) override;

	void FallingRolling(FCollisionQueryParams& Params);

	void ClimbAndStand(FCollisionQueryParams& Params, bool bHitWall);

	

	

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:

	void DecreasePlayerHP(int32 value);
	void IncreasePlayerHP(int32 value);

	bool bEnableDecreasePlayerHP = true;
	bool bEnableIncreasePlayerHP = true;

	void EnableDecreasePlayerHP();
	void EnableIncreasePlayerHP();

	UPROPERTY()
	FTimerHandle bEnableDecreasePlayerHPHandle;

	UPROPERTY()
	FTimerHandle bEnableIncreasePlayerHPHandle;

	void PlayerDie();

	void SetbUseFTimerHandle();
	
	void SetbUseClimbTrue();

	void SetCanClimb(bool value);
	bool GetCanClimb();

	bool bCanClimb = true;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerHP")
	int32 iPlayerMaxHP;

	UPROPERTY(BlueprintReadWrite, Category = "PlayerHP")
	int32 iPlayerHP;

private:
	UPROPERTY()
	UBaseAnimInstance* PlayerAnimInstance;

	FTimerHandle FClimbStandHandle;

	float DefaultBrakingDecelerationFlying;

	float FallingTime = 0.f;
	float RollingTime = 0.f;

//#Wall Change
public:
	UFUNCTION(BlueprintCallable, Category = "ClimbWallChange")
	void ClimbWallChange();

	void OffClimb();

	void StartClimb(FHitResult& HitResult);

	bool CheckWallBehind(float Distance);

//#Climb End Check
public:
	void CheckGroundWhileClimbing();

	FTimerHandle OffClimbTimerHandle;

	void SetOffClimbTrue();
	bool bOffClimb = true;

//#MinimapWidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite ,Category = "MinimapWidget")
	UMinimapWidget* MinimapWidget;


//TogetherRun
public:
	UPROPERTY(BlueprintReadWrite, Category = "Essential Movement Data")
	float fGroundSpeed;

	UPROPERTY()
	ATogetherRunBase* TogetherRunBaseActor;

	float GetfGroundSpeed();
	void SetfGroundSpeedToAniminstance(float value);

	FGetFGroundSpeedTo GetGroundSpeedTo;

	FName HandSocketName = "hand_r_Socket";

	FName GetPlayerRHandSocketName();

	FVector PlayerRightHandLocation;

	FVector GetPlayerRightHandLocation();

};

