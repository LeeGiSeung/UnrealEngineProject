// Fill out your copyright notice in the Description page of Project Settings.


#include "Cable/Cable.h"

#include "CableComponent.h"
#include "Components/BoxComponent.h"

ACable::ACable() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	/* =======================
	   Cable Component (비주얼)
	   ======================= */
	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(Root);

	Cable->CableLength = 300.f;
	Cable->NumSegments = 10;
	Cable->CableWidth = 5.f;

	// 케이블 자체 충돌은 끄는 걸 추천
	Cable->bEnableCollision = false;

	/* =======================
	   Collision Box (막는 용도)
	   ======================= */
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(Root);

	// 대충 케이블 길이만큼
	CollisionBox->SetBoxExtent(FVector(150.f, 20.f, 50.f));

	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_WorldStatic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionBox->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Root 기준 위치
	CollisionBox->SetRelativeLocation(FVector(150.f, 0.f, 0.f));
}
