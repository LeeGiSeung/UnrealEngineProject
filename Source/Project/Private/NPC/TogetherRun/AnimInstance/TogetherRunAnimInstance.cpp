// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/TogetherRun/AnimInstance/TogetherRunAnimInstance.h"
#include "NPC/TogetherRun/TogetherRunBase.h"

void UTogetherRunAnimInstance::SetfTogetherAnimGroundSpeed(float value)
{
	fGroundSpeed = value;
}

void UTogetherRunAnimInstance::SetfTogetherAnimShouldMove(bool value)
{
	bShouldMove = value;
}

void UTogetherRunAnimInstance::SetbTogetherChild(bool value)
{
	bTogetherChild = value;
}

void UTogetherRunAnimInstance::SetNPCReference(ATogetherRunBase* value)
{
	check(value);
	NPC = value;
}

void UTogetherRunAnimInstance::SetLocalFVector(FVector value)
{
	check(NPC);
	UE_LOG(LogTemp, Error, TEXT("value %s"), *value.ToString());
	UE_LOG(LogTemp, Error, TEXT("Mesh World Pos: %s"), *NPC->GetMesh()->GetComponentLocation().ToString());

	LocalFVector = NPC->GetMesh()->GetComponentTransform().InverseTransformPosition(value);

	UE_LOG(LogTemp, Error, TEXT("LocalFvector Target %s"), *LocalFVector.ToString());
}

void UTogetherRunAnimInstance::SetJointTarget(FVector value)
{
	check(NPC);
	//JointTarget = value * NPC->GetActorForwardVector() * 40  * -NPC->GetActorRightVector() * 40;
	JointTarget = value;
	//UE_LOG(LogTemp, Error, TEXT("Joint Target %s"), *JointTarget.ToString());
}

void UTogetherRunAnimInstance::SetHandLocation(FVector Location)
{
	check(NPC);
	LeftHandLocation = Location;
	LocalFVector = NPC->GetMesh()->GetComponentTransform().InverseTransformPosition(LeftHandLocation);

	//JointTarget = NPC->GetActorRightVector() * 30 + NPC->GetActorForwardVector() * 20;
}


