// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill/Earthquake/Earthquake.h"
#include "Components/BoxComponent.h"
#include "ProjectCharacter.h"

AEarthquake::AEarthquake()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	BoxComponent->SetupAttachment(RootComponent);
}

void AEarthquake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEarthquake::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DeleteEarthquake, this, &AEarthquake::OnEarthquakeTimerFinished, 5.f, false);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AEarthquake::OnBlockOverlap);
}

void AEarthquake::OnEarthquakeTimerFinished()
{
	Destroy();
}

void AEarthquake::OnBlockOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;

	if (AProjectCharacter* character = Cast<AProjectCharacter>(OtherActor)) {	
		character->DecreasePlayerHP(1);
	}
}
