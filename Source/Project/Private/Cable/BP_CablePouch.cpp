// Fill out your copyright notice in the Description page of Project Settings.


#include "Cable/BP_CablePouch.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectPlayerController.h"
#include "ProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ABP_CablePouch::ABP_CablePouch()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ABP_CablePouch::BeginPlay()
{
	Super::BeginPlay();

	fInitalPosition = GetActorLocation();
	fInitalRotation = GetActorRotation();

	PlayerController = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	ProjectChacter = Cast<AProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ABP_CablePouch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PlayerController || !ProjectChacter) return;

	if (!PlayerController->GetUseCablePouch() || !GetbUsePouch()) {
		return;
	}

	FVector2D PlayerInput = ProjectChacter->GetMoveInput();
	FVector CurLocation = GetActorLocation();

	// Pouch 기준 방향
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	PlayerInput = -PlayerInput;

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerInput.ToString());

	// 입력을 방향 벡터로 변환
	FVector MoveDir =
		(Right * PlayerInput.Y) +
		(Forward * -(PlayerInput.X));

	MoveDir = MoveDir.GetSafeNormal();

	if (PlayerInput.X != 0) {

		//x 가 1 왼쪽
		if (PlayerInput.X == 1) {
			FRotator DeltaRot(0.f, GetWorld()->GetDeltaSeconds(), 0.f);
			ProjectChacter->AddControllerYawInput(GetWorld()->GetDeltaSeconds());
			AddActorLocalRotation(DeltaRot * 2);
		}
		else if (PlayerInput.X == -1) {
			FRotator DeltaRot(0.f, -GetWorld()->GetDeltaSeconds(), 0.f);
			ProjectChacter->AddControllerYawInput(-GetWorld()->GetDeltaSeconds());
			AddActorLocalRotation(DeltaRot * 2);
		}
	}
	// 최종 위치
	CurLocation += MoveDir * MoveSpeed * GetWorld()->GetDeltaSeconds();

	SetActorLocation(CurLocation);
}

void ABP_CablePouch::UsePouch()
{
	SetbUsePouch(true);
}

void ABP_CablePouch::UnUsePouch()
{
	SetbUsePouch(false);

	ResetPosition();
}

void ABP_CablePouch::FlyPlayer()
{
	FVector CurLocation = GetActorLocation();

	FVector ForwardVector = UKismetMathLibrary::GetForwardVector(GetActorRotation());
	FVector MovementDelta = CurLocation - fInitalPosition;

	UE_LOG(LogTemp, Warning, TEXT("%s"), *MovementDelta.ToString());

	// 이동 벡터를 Forward Vector의 반대 방향과 내적하여 뒷 방향 이동량 계산
	float BackwardAmount = FVector::DotProduct(MovementDelta, -ForwardVector);

	UE_LOG(LogTemp, Warning, TEXT("%f"), BackwardAmount);

	FRotator CameraRot = PlayerController->GetControlRotation();
	FVector Forward =
		FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);

	FVector LaunchVel = Forward * BackwardAmount * 10;

	ProjectChacter->LaunchCharacter(LaunchVel, true, true);

}

void ABP_CablePouch::ResetPosition() {
	SetActorLocation(fInitalPosition);
	SetActorRotation(fInitalRotation);
}