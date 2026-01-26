// Fill out your copyright notice in the Description page of Project Settings.


#include "Cable/BP_CablePouch.h"
#include "Components/StaticMeshComponent.h"
#include "ProjectPlayerController.h"
#include "ProjectCharacter.h"
#include "Kismet/GameplayStatics.h"

ABP_CablePouch::ABP_CablePouch()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ABP_CablePouch::BeginPlay()
{
	Super::BeginPlay();

	fInitalPosition = GetActorLocation();

	PlayerController = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());
	ProjectChacter = Cast<AProjectCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ABP_CablePouch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController->GetUseCablePouch() == false) {
		return;
	}

	if (!PlayerController || !ProjectChacter) return;

	FVector2D PlayerInput = -(ProjectChacter->GetMoveInput());
	FVector CurLocation = GetActorLocation();

	CurLocation.X += PlayerInput.X;
	CurLocation.Y += PlayerInput.Y;

	if (PlayerInput.X == 0 && PlayerInput.Y == 0) return;
	SetActorLocation(CurLocation);
}

void ABP_CablePouch::UsePouch(FVector2D _Value)
{

}
