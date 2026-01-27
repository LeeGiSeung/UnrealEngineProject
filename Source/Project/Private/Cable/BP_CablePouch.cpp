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
	if (!PlayerController || !ProjectChacter) return;

	if (!PlayerController->GetUseCablePouch() || !GetbUsePouch()) {
		return;
	}

	FVector2D PlayerInput = ProjectChacter->GetMoveInput();
	FVector CurLocation = GetActorLocation();

	// Pouch 기준 방향
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	// 입력을 방향 벡터로 변환
	FVector MoveDir =
		(Forward * PlayerInput.Y) +
		(Right * PlayerInput.X);

	// 이동량 (속도 조절)
	float MoveSpeed = 100.f;

	// 최종 위치
	CurLocation += MoveDir * MoveSpeed * GetWorld()->GetDeltaSeconds();

	SetActorLocation(CurLocation);
}

void ABP_CablePouch::UsePouch()
{

	SetbUsePouch(true);

	//플레이어의 상하 시야각 + Pouch의 기준 위치에서 좌우 위치 = 플레이어가 날라갈 위치
	
	UE_LOG(LogTemp, Warning, TEXT("right vector : %s"), *GetActorRightVector().ToString());
}

void ABP_CablePouch::UnUsePouch()
{
	SetbUsePouch(false);
}
