// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/TogetherRun/TogetherRunBase.h"

// Sets default values
ATogetherRunBase::ATogetherRunBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATogetherRunBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATogetherRunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATogetherRunBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

