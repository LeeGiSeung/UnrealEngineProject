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

}

void ABP_CablePouch::UsePouch()
{
	SetbUsePouch(true);

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
	SetbUsePouch(false);

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	ResetPosition();
}

void ABP_CablePouch::FlyPlayer()
{
	if (!ProjectChacter) return;

	float PullPower = ProjectChacter->GetPouchPush();

	FRotator CameraRot = PlayerController->GetControlRotation();
	FVector LaunchDir = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);
		
	FVector LaunchVel = LaunchDir * PullPower * 4;

	ProjectChacter->LaunchCharacter(LaunchVel, true, true);

	ProjectChacter->ResetPouch();

	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
}

void ABP_CablePouch::ResetPosition() {
	SetActorLocation(fInitalPosition);
	SetActorRotation(fInitalRotation);

	ProjectChacter->ResetPouch();
}