// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TogetherManager/TogetherManager.h"
#include "NPC/TogetherRun/TogetherRunBase.h"
#include "ProjectCharacter.h"

#include "NPC/TogetherRun/AnimInstance/TogetherRunAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "City/MapWidget/Marker/MapViewer/TogetherActor/TogetherActorMarker.h"

void UTogetherManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UTogetherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void UTogetherManager::AddChainArray(ATogetherRunBase* value)
{
	if (!value) return;
	if (ChainArray.Contains(value)) return; //중복 검사

	if (ChainArray.Num() >= 1) {
		Cast<UTogetherRunAnimInstance>(ChainArray[ChainArray.Num() - 1]->GetMesh()->GetAnimInstance())->SetbTogetherChild(true);
	}

	ChainArray.Add(value);

	if (value->TogetherActorMarker) {
		value->TogetherActorMarker->RemoveFromParent();
	}
	

}

void UTogetherManager::RemoveChainArray(ATogetherRunBase* value)
{
	ChainArray.Remove(value);
}

int UTogetherManager::GetChainArrayNum()
{
	return ChainArray.Num();
}

void UTogetherManager::RegisterPlayer(AProjectCharacter* value)
{
	Player = value;
}

void UTogetherManager::PlaceChainArray(const float speed, const FVector HandLocation) 
{
	float fSpeed = speed;

	int32 index = 0;

	float DistanceBetweenNPCs = 65.0f;

	for (ATogetherRunBase* actor : ChainArray) {

		actor->GetCharacterMovement()->MaxWalkSpeed = fSpeed;
		UTogetherRunAnimInstance* AnimInst = Cast<UTogetherRunAnimInstance>(actor->GetMesh()->GetAnimInstance());

		AnimInst->SetfTogetherAnimGroundSpeed(fSpeed);
		AnimInst->SetfTogetherAnimShouldMove(fSpeed > 0);

		FVector ExtraRightOffset = Player->GetActorRightVector() * 15.0f;
		FVector ExtraBackOffset = -Player->GetActorForwardVector() * DistanceBetweenNPCs * (index + 1);

		FVector NewLocation = Player->GetActorLocation() + ExtraRightOffset + ExtraBackOffset;

		actor->SetActorLocation(NewLocation);


		FVector TargetHandLocation;
		if (index == 0) {
			TargetHandLocation = Player->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName);
		}
		else {
			TargetHandLocation = ChainArray[index - 1]->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName);
		}

		AnimInst->SetLocalFVector(TargetHandLocation);

		FVector MyShoulderLocation = actor->GetMesh()->GetSocketLocation(TogetherNPCLeftHandName);

		FVector FinalJointTarget = MyShoulderLocation;

		AnimInst->SetJointTarget(FinalJointTarget);

		Cast<UTogetherRunAnimInstance>(actor->GetMesh()->GetAnimInstance())->SetAnimPlayRate(FMath::FRandRange(0.5, 1)); //애니메이션 속도 조정

		index++;
	}
}

void UTogetherManager::RegisterTogetherActor(ATogetherRunBase* value)
{
	WorldSpawnChainArray.Add(value);
}

TArray<ATogetherRunBase*>* UTogetherManager::GetWorldSpawnChainArray()
{
	return &WorldSpawnChainArray;
}

TArray<ATogetherRunBase*> UTogetherManager::GetChainArray()
{
	return ChainArray;
}

TArray<ATogetherRunBase*>& UTogetherManager::GetChainReferenceArray()
{
	return ChainArray;
}

