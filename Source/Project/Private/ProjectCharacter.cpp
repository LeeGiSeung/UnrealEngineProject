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
	FVector End = Start + (GetActorForwardVector() * 120.0f); // 그냥 항상 앞으로만 쏘면 됩니다.
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHitWall = false;

	// WallChange 중일 때는 굳이 전방 트레이스 연산을 할 필요도 없습니다.
	if (!PlayerAnimInstance->GetWallChange() && GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		if (FMath::Abs(HitResult.Normal.Z) < 0.2f)
		{
			bHitWall = true;
		}
	}
	//else {
	//	if (PlayerAnimInstance->GetIsClimb()) {
	//		if (PlayerAnimInstance->GetbWallUpStandTo()) {
	//			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	//			GetCharacterMovement()->bOrientRotationToMovement = true;
	//			OffClimb();
	//		}
	//	}
	//}


	if (bHitWall && !PlayerAnimInstance->GetIsClimb() && GetCanClimb() && !PlayerAnimInstance->GetWallChange() && bOffClimb)
	{
		StartClimb(HitResult);
	}
	else {

	}
	

	// 2. 이미 벽을 타고 있는 중일 때의 로직 (자동 올라가기 체크)
	if (PlayerAnimInstance->GetIsClimb())
	{
		ClimbAndStand(Params, bHitWall);
	}

	if (PlayerAnimInstance->GetIsCppFalling()) {
		FallingTime += DeltaTime;
	}

	FallingRolling(Params);
	
	if (!PlayerAnimInstance->GetIsCppFalling()) {
		FallingTime = 0.f;
	}

	CheckGroundWhileClimbing();

}

void AProjectCharacter::FallingRolling(FCollisionQueryParams& Params)
{
	FHitResult RollingHit;
	FVector RollingStart = GetActorLocation();
	FVector RollingEnd = RollingStart - FVector(0.f, 0.f, 300.f); // 아래로 쏨

	FCollisionQueryParams RollingParams;
	Params.AddIgnoredActor(this);

	bool RollingbHit = GetWorld()->LineTraceSingleByChannel(
		RollingHit,
		RollingStart,
		RollingEnd,
		ECC_Visibility,
		RollingParams
	);

	if (RollingbHit)
	{
		float DistanceToGround = (RollingStart - RollingHit.Location).Size();

		if (DistanceToGround <= 300.f)
		{
			if (FallingTime > 1.f)
			{
				UE_LOG(LogTemp, Error, TEXT("Falling"));
				PlayerAnimInstance->SetIsFullFalling(true);
				FallingTime = 0.f;
			}

		}
	}
}

void AProjectCharacter::ClimbAndStand(FCollisionQueryParams& Params, bool bHitWall)
{

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
	const float AnimationClimbHeight = 150.0f;

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
			StandUpTo();
			SetbUseFTimerHandle();
		}
	}
	// 3. 만약 벽 꼭대기도 없고 앞쪽 벽도 없다면 (벽의 옆으로 벗어났을 때 등)
	else if (!bHitWall && !PlayerAnimInstance->GetIsClimbStand())
	{
		// 벽에서 벗어났으므로 물리 모드를 Falling으로 변경
		GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		OffClimb();
	}
}

void AProjectCharacter::StartClimb(FHitResult& HitResult)
{
	
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	GetCharacterMovement()->bOrientRotationToMovement = false;

	PlayerAnimInstance->SetbIsClimb(true);
	PlayerAnimInstance->SetClimbInputXY(FVector2D(0.5, 0.5));

	FVector WallNormal = HitResult.Normal;

	// 기울어짐 방지 (Z축 수평 맞추기)
	WallNormal.Z = 0.0f;
	WallNormal.Normalize();

	// WallChange든 일반 등반이든 벽을 마주보도록 회전해야 함
	FRotator TargetRotation = (-WallNormal).Rotation();
	SetActorRotation(TargetRotation);

	// 위치 보정
	float Distance = GetCapsuleComponent()->GetScaledCapsuleRadius();
	FVector NewLocation = HitResult.ImpactPoint + (WallNormal * Distance) + FVector(0.0f, 0.0f, 15.0f);;

	// 일반 등반일 때만 SetActorLocation으로 벽에 즉시 붙여줌
	// (WallChange 애니메이션 도중에는 강제 이동시키면 애니메이션이 튈 수 있으므로 제외)
	if (!PlayerAnimInstance->GetWallChange()) {
		SetActorLocation(NewLocation);
	}

	PlayerAnimInstance->SetWallChangeLocation(NewLocation, TargetRotation);

}

void AProjectCharacter::OffClimb()
{
	SetCanClimb(true);
	PlayerAnimInstance->SetbIsClimb(false);


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

void AProjectCharacter::StandUpTo()
{
	if (PlayerAnimInstance->GetIsClimb()) {

		//UE_LOG(LogTemp, Warning, TEXT("END CLIMB"));

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
	//UE_LOG(LogTemp, Warning, TEXT("DASF"));
}

void AProjectCharacter::ClimbWallChange() {
	if (!PlayerAnimInstance->GetIsClimb() || PlayerAnimInstance->GetWallChange()) return;

	// 2. 뒤에 300 유닛 안에 벽이 없으면 리턴 (추가된 조건)
	if (!CheckWallBehind(250.f))
	{
		UE_LOG(LogTemp, Warning, TEXT("No wall behind to jump to!"));
		return;
	}

	PlayerAnimInstance->SetWallChange(true);
}

bool AProjectCharacter::CheckWallBehind(float Distance)
{
	FHitResult HitResult;
	FVector Start = GetActorLocation();
	// 캐릭터의 뒤쪽 방향으로 Distance만큼 쏩니다.
	FVector End = Start + (-GetActorForwardVector() * Distance);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// 라인트레이스 실행
	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
	{
		// 수직에 가까운 벽인지 확인 (노멀의 Z값이 낮아야 함)
		if (FMath::Abs(HitResult.Normal.Z) < 0.2f)
		{
			// 디버그용 (벽이 있을 때 초록색 선)
			DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 2.0f);
			return true;
		}
	}

	// 벽이 없거나 수직이 아님 (빨간색 선)
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 2.0f);
	return false;
}

void AProjectCharacter::CheckGroundWhileClimbing()
{
	// 현재 클라이밍 상태가 아니라면 실행하지 않음
	if (!PlayerAnimInstance->GetIsClimb()) return;

	// 캐릭터의 현재 위치 (일반적으로 캡슐 컴포넌트의 중앙)
	FVector StartLocation = GetActorLocation();

	// 캡슐의 절반 높이에 여유 공간(Threshold)을 더해 트레이스 길이를 설정
	float CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float GroundCheckThreshold = 10.0f; // 바닥으로 인식할 여유 거리 (필요에 따라 조절)
	float TraceDistance = CapsuleHalfHeight + GroundCheckThreshold;

	FVector EndLocation = StartLocation - FVector(0.0f, 0.0f, TraceDistance);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this); // 자기 자신은 충돌에서 제외

	// 아래 방향으로 라인 트레이스 실행
	bool bHitGround = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		ECC_Visibility, // 또는 바닥 전용 콜리전 채널 (예: ECC_WorldStatic)
		CollisionParams
	);

	// 디버그 라인 (테스트용, 확인 후 삭제)
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, bHitGround ? FColor::Green : FColor::Red, false, -1.0f, 0, 2.0f);

	if (bHitGround)
	{
		// 바닥이 감지되었으므로 클라이밍 종료
		OffClimb();
		bOffClimb = false;
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetWorld()->GetTimerManager().SetTimer(OffClimbTimerHandle, this, &AProjectCharacter::SetOffClimbTrue, 1.f, false);
	}
}

void AProjectCharacter::SetOffClimbTrue()
{
	bOffClimb = true;
}
