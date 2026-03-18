// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "ProjectPlayerController.h"
#include "Cable/BP_CablePouch.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "BaseAnimInstance.h"

#include "GameFramework/Controller.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AProjectCharacter

AProjectCharacter::AProjectCharacter()
{

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// CameraAnchor (기준점)
	CameraAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("CameraAnchor"));
	CameraAnchor->SetupAttachment(GetCapsuleComponent());
	CameraAnchor->SetRelativeLocation(FVector(0.f, 0.f, 160.f));

	// FaceCameraAnchor (방향 기준)
	FaceCameraAnchor = CreateDefaultSubobject<USceneComponent>(TEXT("FaceCameraAnchor"));
	FaceCameraAnchor->SetupAttachment(CameraAnchor);

	// CameraBoom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(FaceCameraAnchor);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// FollowCamera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

}

void AProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* Pc = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Pc->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	PlayerController = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	
	DirectingManager =
		Cast<ADirectingManager>(
			UGameplayStatics::GetActorOfClass(
				GetWorld(),
				ADirectingManager::StaticClass()
			)
		);

	DirectingManager->RegisterSequenceActor(FName("Player"), this);

	PlayerAnimInstance = Cast<UBaseAnimInstance>(GetMesh()->GetAnimInstance());

}

void AProjectCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 70.0f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		//UE_LOG(LogTemp, Error, TEXT("CLIMB"));
		// 벽면의 각도가 수직에 가까운지 체크 (Normal.Z가 0 근처)
		if (FMath::Abs(HitResult.Normal.Z) < 0.1f)
		{
			PlayerAnimInstance->SetbIsClimb(true);
			// 이동 모드를 Flying으로 바꾸면 중력 영향을 안 받고 벽에 붙어있을 수 있습니다.
			GetCharacterMovement()->SetMovementMode(MOVE_Flying);

			PlayerAnimInstance->SetClimbInputXY(FVector2D(0.5,0.5));

			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else {
		//UE_LOG(LogTemp, Error, TEXT("NO CLIMB"));
		//PlayerAnimInstance->SetbIsClimb(false);
	}

}

void AProjectCharacter::DecreasePlayerHP(int32 value)
{
	
	if (!bEnableDecreasePlayerHP) return;

	iPlayerHP -= value;

	UE_LOG(LogTemp, Error, TEXT("DecraseHp"));

	GetWorldTimerManager().SetTimer(bEnableDecreasePlayerHPHandle, this, &AProjectCharacter::EnableDecreasePlayerHP, 0.5f, false);

	bEnableDecreasePlayerHP = false;

	if (iPlayerHP <= 0) {
		PlayerDie();
	}
}

void AProjectCharacter::IncreasePlayerHP(int32 value)
{
	if (!bEnableIncreasePlayerHP) return;

	GetWorldTimerManager().SetTimer(bEnableIncreasePlayerHPHandle, this, &AProjectCharacter::EnableIncreasePlayerHP, 0.5f, false);

	iPlayerHP += value;
}

void AProjectCharacter::EnableDecreasePlayerHP()
{
	bEnableDecreasePlayerHP = true;
}

void AProjectCharacter::EnableIncreasePlayerHP()
{
	bEnableIncreasePlayerHP = true;
}

void AProjectCharacter::PlayerDie()
{
	UE_LOG(LogTemp, Error, TEXT("PLAYER DIE"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectCharacter::OnSpacePressed()
{
	if (PlayerController->GetUseCablePouch()) return;

	Jump();
}

void AProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AProjectCharacter::OnSpacePressed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AProjectCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AProjectCharacter::Look);

		EnhancedInputComponent->BindAction(SitAction, ETriggerEvent::Started, this, &AProjectCharacter::Sit);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AProjectCharacter::Move(const FInputActionValue& Value)
{

	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (PlayerController->GetUseCablePouch()) {

		//float LimitExtend = PlayerController->GetUsingPouch()->GetLimitExtent();

		SetMoveInput(MovementVector);
		if (MovementVector.Y == 1) {
			if (PouchPush > 0) { //PouchPush 각 Pouch가 관리하도록 변경해야함 
				PouchPush -= 1;
			}
		}
		else if (MovementVector.Y == -1) {
			PouchPush += 1;
		}
	}
	
	if (Controller != nullptr)
	{

		if (PlayerAnimInstance->GetIsClimb()) {

			float TargetX = (MovementVector.X * 0.5f) + 0.5;
			float TargetY = (MovementVector.Y * 0.5f) + 0.5;

			UE_LOG(LogTemp, Error, TEXT("TargetX : %f, TargetY : %f"), TargetX, TargetY);

			PlayerAnimInstance->SetClimbInputXY(FVector2D(TargetX, TargetY));

			//if (TargetY == 0.5f) TargetY = 1.f;
			//if (TargetX == 0.5f) TargetX = 1.f;

			// 2. 실제 캐릭터 이동 처리
			// 클라이밍 상태에서는 캐릭터의 Up 벡터가 '위', Right 벡터가 '오른쪽'이 됩니다.
			const FVector UpDirection = GetActorUpVector();
			const FVector RightDirection = GetActorRightVector();

			MovementVector = MovementVector / 5;

			// 입력값에 따라 이동 명령 전달
			
			// MovementVector.Y (W/S) -> 위/아래 이동
			AddMovementInput(UpDirection, MovementVector.Y);
			// MovementVector.X (A/D) -> 왼쪽/오른쪽 이동
			AddMovementInput(RightDirection, MovementVector.X);

		}
		else {
			//UE_LOG(LogTemp, Error, TEXT("Normal Move"));
			// find out which way is forward
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get forward vector
			const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			// get right vector 
			const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			// add movement 
			AddMovementInput(ForwardDirection, MovementVector.Y);
			AddMovementInput(RightDirection, MovementVector.X);
		}
	}
}

void AProjectCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr && !bIsDrawing && !PlayerAnimInstance->GetIsClimb())
	{
		AddControllerPitchInput(LookAxisVector.Y);
		// add yaw and pitch input to controller
		if (PlayerController->GetUseCablePouch()) return;
		AddControllerYawInput(LookAxisVector.X);
	}
}

void AProjectCharacter::Sit(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("DASF"));
}