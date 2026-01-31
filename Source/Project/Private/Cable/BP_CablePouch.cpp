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
	RootComponent = Mesh;

	Mesh->SetSimulatePhysics(false);

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

	if (bSuccesFly) {
		FlyDistance += GetWorld()->GetDeltaSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f"), FlyDistance);
		if (FlyDistance >= MinFlyPower / 100) {
			bSuccesFly = false;
			ResetPosition();
		}
	}

	if (!PlayerController->GetUseCablePouch() || !GetbUsePouch()) {
		return;
	}

	FVector2D PlayerInput = ProjectChacter->GetMoveInput();

	int InputX = PlayerInput.X;

	if (InputX == -1) {
		ProjectChacter->AddControllerYawInput(1.f * GetWorld()->GetDeltaSeconds() * correction);
	}
	else if (InputX == 1) {
		ProjectChacter->AddControllerYawInput(-1.f * GetWorld()->GetDeltaSeconds() * correction);
	}

	if (GetbUsePouch()) {
		Distance = FVector::Dist(fInitalPosition, GetActorLocation());
		
		if (Distance > 20.f && !bHasMovedAway) // 먼저 충분히 멀어져야 함
		{
			bHasMovedAway = true;
		}

		if (Distance < 20.f && bHasMovedAway) {
			UnUsePouch();
		}
	}

}

void ABP_CablePouch::UsePouch()
{
	SetbUsePouch(true);
	UE_LOG(LogTemp, Warning, TEXT("UsePouch"));
	if (ProjectChacter)
	{
		AttachToComponent(
			ProjectChacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			TEXT("CablePouch") // 소켓 이름
		);
	}

}

void ABP_CablePouch::UnUsePouch()
{
	if (!PlayerController) {
		UE_LOG(LogTemp, Warning, TEXT("NONE PLAYERCONTROLLER"));
		return;
	}

	if (GetbUsePouch() == false) {
		UE_LOG(LogTemp, Warning, TEXT("UsePouch False"));
		return;
	}

	SetbUsePouch(false);

	bHasMovedAway = false;

	ResetPosition();

	PlayerController->UnUseCable();
}

void ABP_CablePouch::FlyPlayer()
{
	if (!ProjectChacter || !PlayerController) return;
	if (ProjectChacter->GetPouchPush() < MinFlyPower) {
		UE_LOG(LogTemp, Warning, TEXT("%f min distance"), Distance);
	}

	float PullPower = ProjectChacter->GetPouchPush();

	FRotator CameraRot = PlayerController->GetControlRotation();
	FVector LaunchDir = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);
		
	FVector LaunchVel = LaunchDir * PullPower * 4;

	ProjectChacter->LaunchCharacter(LaunchVel, true, true);

	bSuccesFly = true;
}

void ABP_CablePouch::ResetPosition() {

	SetActorLocation(fInitalPosition);
	SetActorRotation(fInitalRotation);

	ProjectChacter->ResetPouch();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	UnUsePouch();
}