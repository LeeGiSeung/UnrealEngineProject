// Fill out your copyright notice in the Description page of Project Settings.


#include "BurnActor/BurnActor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

ABurnActor::ABurnActor()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;

	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABurnActor::BeginPlay()
{
	Super::BeginPlay();

	DissolveMID = MeshComp->CreateAndSetMaterialInstanceDynamic(0);

}

void ABurnActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsBurn) {
		CurTime += DeltaTime;

		UE_LOG(LogTemp, Warning, TEXT("%f"), CurTime);

		if (CurTime >= BurnTime) {
			if (DissolveMID)
			{
				float CurBurn = 0.f;
				bool bIsGetScalar = DissolveMID->GetScalarParameterValue(
					FMaterialParameterInfo(TEXT("DissolveAmount")),
					CurBurn
				);

				if (CurBurn >= 1) Destroy();

				DissolveMID->SetScalarParameterValue("DissolveAmount", CurBurn + t * DeltaTime);
			}
		}
	}
}

void ABurnActor::SetIsBurn()
{
	bIsBurn = true;
}

bool ABurnActor::GetIsBurn()
{
	return bIsBurn;
}
