// Fill out your copyright notice in the Description page of Project Settings.


#include "Drawing_Tree_Actor.h"
#include "Manager/DrawingActorManager.h"
#include "EngineUtils.h"
ADrawing_Tree_Actor::ADrawing_Tree_Actor()
{
	PrimaryActorTick.bCanEverTick = true;
	RenderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	//RenderMesh->SetHiddenInGame(true);          // 안 보이게
	//RenderMesh->SetVisibility(false);           // 안전빵
	//RenderMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	//RenderMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	//RenderMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	//RenderMesh->SetCollisionObjectType(ECC_WorldDynamic);

}

void ADrawing_Tree_Actor::Tick(float DeltaTime)
{
	if (!isGrow) return;
	if (CurTime >= DuarationTime)
	{
		DrawingManager->DeleteDrawingActor(this);
		Destroy();
	}

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
