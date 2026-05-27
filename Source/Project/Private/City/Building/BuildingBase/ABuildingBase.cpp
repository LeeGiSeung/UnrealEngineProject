// Fill out your copyright notice in the Description page of Project Settings.


#include "City/Building/BuildingBase/ABuildingBase.h"

// Sets default values
AABuildingBase::AABuildingBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AABuildingBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

