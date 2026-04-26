// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/TogetherManager/TogetherManager.h"
#include "NPC/TogetherRun/TogetherRunBase.h"
#include "ProjectCharacter.h"

#include "NPC/TogetherRun/AnimInstance/TogetherRunAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	if (ChainArray.Contains(value)) return; //이미 있으면 추가안함

	if (ChainArray.Num() >= 1) {
		Cast<UTogetherRunAnimInstance>(ChainArray[ChainArray.Num() - 1]->GetMesh()->GetAnimInstance())->SetbTogetherChild(true);
	}

	ChainArray.Add(value);


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

void UTogetherManager::PlaceChainArray(const float speed, const FVector HandLocation) //HandLocation : PlayerHandLocation
{
	int i = 1;
	float fSpeed = speed; //현재 PlayerSpeed각 Actor들한테 이 Speeㅇ넘겨주면됨
	//첫번째는 Player

	for (ATogetherRunBase* actor : ChainArray) {
		FVector LocalFVector, JointTarget;
		
		actor->GetCharacterMovement()->MaxWalkSpeed = fSpeed;

		FVector NPCHaneSocketNameLocation = actor->GetMesh()->GetSocketLocation(TogetherNPCLeftHandName);

		// 차이 계산
		FVector Offset = HandLocation - NPCHaneSocketNameLocation;

		FVector ExtraRightOffset = Player->GetActorRightVector() * 10.0f;
		FVector ExtraBackOffset = -Player->GetActorForwardVector() * 60.0f * i;

		// NPC 이동
		FVector ActorLocation = Player->GetActorLocation() + ExtraRightOffset + ExtraBackOffset;

		actor->SetActorLocation(ActorLocation);

		UTogetherRunAnimInstance* TogetherRunAnimInstance = Cast<UTogetherRunAnimInstance>(actor->GetMesh()->GetAnimInstance());

		TogetherRunAnimInstance->SetfTogetherAnimGroundSpeed(fSpeed);
		TogetherRunAnimInstance->SetfTogetherAnimShouldMove(fSpeed > 0);

		FVector PlayerHandLocation = HandLocation;

		UE_LOG(LogTemp, Error, TEXT("array index : %d"), i);

		TogetherRunAnimInstance->SetJointTarget(PlayerHandLocation);

		if (i == 1) {
			TogetherRunAnimInstance->SetLocalFVector(Player->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName));
		}
		else {
			TogetherRunAnimInstance->SetLocalFVector(ChainArray[i-2]->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName));
		}

		

		i++;
	}
}

TArray<ATogetherRunBase*> UTogetherManager::GetChainArray()
{
	return ChainArray;
}

