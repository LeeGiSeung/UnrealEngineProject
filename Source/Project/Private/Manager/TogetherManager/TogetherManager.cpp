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
	float fSpeed = speed; //현재 PlayerSpeed각 Actor들한테 이 Speeㅇ넘겨주면됨

	int32 index = 0;
	//// NPC 사이의 물리적 간격 (캡슐 크기에 맞춰 조절)
	float DistanceBetweenNPCs = 65.0f;

	for (ATogetherRunBase* actor : ChainArray) {
		// 1. 이동 속도 및 애니메이션 기본 설정
		actor->GetCharacterMovement()->MaxWalkSpeed = fSpeed;
		UTogetherRunAnimInstance* AnimInst = Cast<UTogetherRunAnimInstance>(actor->GetMesh()->GetAnimInstance());

		AnimInst->SetfTogetherAnimGroundSpeed(fSpeed);
		AnimInst->SetfTogetherAnimShouldMove(fSpeed > 0);

		// 2. NPC 본체 위치 설정 (이게 먼저 수행되어야 함)
		// Offset.Z 대신 고정된 거리값(DistanceBetweenNPCs)을 사용하세요.
		FVector ExtraRightOffset = Player->GetActorRightVector() * 15.0f;
		FVector ExtraBackOffset = -Player->GetActorForwardVector() * DistanceBetweenNPCs * (index + 1);

		// Z값은 플레이어의 위치를 그대로 따라가게 하여 바닥 높이를 맞춥니다.
		FVector NewLocation = Player->GetActorLocation() + ExtraRightOffset + ExtraBackOffset;
		//actor->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		actor->SetActorLocation(NewLocation);

		// 3. IK 타겟 설정 (손 위치)
		FVector TargetHandLocation;
		if (index == 0) {
			// 첫 번째 NPC는 플레이어의 오른쪽 손 소켓을 잡음
			TargetHandLocation = Player->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName);
		}
		else {
			// 두 번째부터는 바로 앞 NPC(index - 1)의 오른쪽 손 소켓을 잡음
			TargetHandLocation = ChainArray[index - 1]->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName);
		}

		AnimInst->SetLocalFVector(TargetHandLocation);

		FVector MyShoulderLocation = actor->GetMesh()->GetSocketLocation(TogetherNPCLeftHandName);

		// 내 어깨 위치에서 뒤로 30, 위로 20 지점을 팔꿈치가 바라보게 합니다.
		FVector FinalJointTarget = MyShoulderLocation;

		AnimInst->SetJointTarget(FinalJointTarget);

		Cast<UTogetherRunAnimInstance>(actor->GetMesh()->GetAnimInstance())->SetAnimPlayRate(FMath::FRandRange(0.2, 1)); //애니메이션 속도 조정

		index++;
	}

//JointTarget Error
//LogTemp : Error: index: 0, FinalJointTarget : X = -593.248 Y = -2467.053 Z = 92.234
//LogTemp : Error : index : 1, FinalJointTarget : X = -590.501 Y = -2399.742 Z = 95.193
//LogTemp : Error : index : 2, FinalJointTarget : X = -575.107 Y = -2336.591 Z = 95.193
//LogTemp : Error : index : 3, FinalJointTarget : X = -559.712 Y = -2273.441 Z = 95.193
//LogTemp : Error : index : 4, FinalJointTarget : X = -544.318 Y = -2210.290 Z = 95.193
//LogTemp : Error : index : 5, FinalJointTarget : X = -528.923 Y = -2147.139 Z = 95.193
//LogTemp : Error : index : 6, FinalJointTarget : X = -513.529 Y = -2083.989 Z = 95.193
//LogTemp : Error : index : 7, FinalJointTarget : X = -498.134 Y = -2020.838 Z = 95.193


}

TArray<ATogetherRunBase*> UTogetherManager::GetChainArray()
{
	return ChainArray;
}

