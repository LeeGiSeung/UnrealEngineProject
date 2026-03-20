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

	if (GetCharacterMovement())
	{
		DefaultBrakingDecelerationFlying = GetCharacterMovement()->BrakingDecelerationFlying;
	}

}

void AProjectCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 120.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHitWall = false;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		if (FMath::Abs(HitResult.Normal.Z) < 0.2f)
		{
			bHitWall = true;
		}
	}

	// 1. 벽에 닿았고, 아직 등반 상태가 아니며, 쿨타임(GetCanClimb)이 돌았을 때 진입
	if (bHitWall && !PlayerAnimInstance->GetIsClimb() && GetCanClimb())
	{
		UE_LOG(LogTemp, Warning, TEXT("START CLIMB"));

		PlayerAnimInstance->SetbIsClimb(true);
		PlayerAnimInstance->SetClimbInputXY(FVector2D(0.5, 0.5));

		GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		GetCharacterMovement()->bOrientRotationToMovement = false;

		FRotator TargetRotation = (HitResult.Normal * -1.f).Rotation();
		SetActorRotation(TargetRotation);
	}

	// 2. 이미 벽을 타고 있는 중일 때의 로직 (자동 올라가기 체크)
	if (PlayerAnimInstance->GetIsClimb())
	{
		//UE_LOG(LogTemp, Error, TEXT("%s"), *GetLastMovementInputVector().ToString());

		if (GetLastMovementInputVector().Y == 0.f && GetLastMovementInputVector().Z == 0)
		{
			// 애니메이션 파라미터도 중립으로 서서히 돌려보내고 싶다면 여기서 처리
			PlayerAnimInstance->SetClimbInputXY(FVector2D(0.5f, 0.5f));
		}

		// 캡슐 중심에서 위로 올린 후 앞(벽 쪽)으로 조금 이동한 곳에서 아래로 트레이스
		FVector LedgeTraceStart = GetActorLocation() + FVector(0.f, 0.f, 150.f) + (GetActorForwardVector() * 50.f);
		FVector LedgeTraceEnd = LedgeTraceStart - FVector(0.f, 0.f, 200.f);
		FHitResult LedgeHit;

		bool bHitLedge = GetWorld()->LineTraceSingleByChannel(LedgeHit, LedgeTraceStart, LedgeTraceEnd, ECC_Visibility, Params);

		// 1. 애니메이션이 실제로 올라가는 높이 (에디터에서 확인 후 입력)
		const float AnimationClimbHeight = 210.0f;

		if (bHitLedge && !PlayerAnimInstance->GetIsClimbStand())
		{
			
			float LedgeZ = LedgeHit.ImpactPoint.Z;
			float CharacterFootZ = GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

			// 캐릭터의 '발' 위치에서 벽 꼭대기까지의 실제 거리
			float VerticalDistanceToLedge = LedgeZ - CharacterFootZ;

			// 2. 오차 범위 설정 (약 5~10 unit 정도의 여유)
			float ErrorMargin = 10.0f;

			//UE_LOG(LogTemp, Error, TEXT("LedgeZ : %f ChatercterFootZ : %f VertiicalDeistanceToLEDGE : %f"), LedgeZ, CharacterFootZ, VerticalDistanceToLedge);

			// 현재 남은 거리가 애니메이션이 올라갈 높이와 거의 일치할 때 실행
			if (FMath::IsWithin(VerticalDistanceToLedge, AnimationClimbHeight - 5.f, AnimationClimbHeight + 5.f))
			{
				GetCharacterMovement()->StopMovementImmediately();
				// 상태 전환 및 타이머 실행
				TestSetStand();
				SetbUseFTimerHandle();
			}
		}
		// 3. 만약 벽 꼭대기도 없고 앞쪽 벽도 없다면 (벽의 옆으로 벗어났을 때 등)
		else if (!bHitWall && !PlayerAnimInstance->GetIsClimbStand())
		{
			PlayerAnimInstance->SetbIsClimb(false);
			GetCharacterMovement()->SetMovementMode(MOVE_Walking);
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
	}

	if (PlayerAnimInstance->GetIsFalling()) {
		FallingTime += DeltaTime;
	}

	if (FallingTime > 1.f) {
		if (PlayerAnimInstance->GetIsFalling()) {
			PlayerAnimInstance->SetIsFullFalling(true);
			FallingTime = 0.f;
		}
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

void AProjectCharacter::SetbUseFTimerHandle()
{
	GetWorldTimerManager().SetTimer(FClimbStandHandle, this, &AProjectCharacter::SetbUseClimbTrue, 2.f, false);
}

void AProjectCharacter::SetCanClimb(bool value)
{
	bCanClimb = value;
}

void AProjectCharacter::SetbUseClimbTrue()
{
	SetCanClimb(true);
	PlayerAnimInstance->SetbIsClimb(false);
}

bool AProjectCharacter::GetCanClimb()
{
	return bCanClimb;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AProjectCharacter::OnSpacePressed()
{
	if (PlayerController->GetUseCablePouch()) return;

	Jump();
}

void AProjectCharacter::TestSetStand()
{
	if (PlayerAnimInstance->GetIsClimb()) {

		UE_LOG(LogTemp, Warning, TEXT("END CLIMB"));

		PlayerAnimInstance->SetbIsClimbStand(true);
	}

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

			//UE_LOG(LogTemp, Error, TEXT("TargetX : %f, TargetY : %f"), TargetX, TargetY);

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
	// && !PlayerAnimInstance->GetIsClimb()
	if (Controller != nullptr && !bIsDrawing)
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