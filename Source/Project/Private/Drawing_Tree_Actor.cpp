// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawing_Tree_Actor.h"

ADrawing_Tree_Actor::ADrawing_Tree_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

void ADrawing_Tree_Actor::Tick(float DeltaTime)
{
	if (!isGrow) return;
	if (CurTime >= DuarationTime) Destroy();

	Super::Tick(DeltaTime);

	CurTime += DeltaTime;

	GrowTree();
}

void ADrawing_Tree_Actor::GrowTree()
{
	if (CurTime > GrowTime) return; //성장 시간 넘기면 return
	FVector NewScale = GetActorScale3D();
	NewScale.Z = NewScale.Z + GrowSpeed * GetWorld()->GetDeltaSeconds();
	SetActorScale3D(NewScale);
}

void ADrawing_Tree_Actor::BeginPlay()
{
	Super::BeginPlay();
}

void ADrawing_Tree_Actor::UseAbility()
{
	Super::UseAbility();

	isGrow = true;
}
