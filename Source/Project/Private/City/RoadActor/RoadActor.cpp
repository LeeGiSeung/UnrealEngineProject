// Fill out your copyright notice in the Description page of Project Settings.


#include "City/RoadActor/RoadActor.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ARoadActor::ARoadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	RootComponent = SplineMesh;

}

// Called when the game starts or when spawned
void ARoadActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

