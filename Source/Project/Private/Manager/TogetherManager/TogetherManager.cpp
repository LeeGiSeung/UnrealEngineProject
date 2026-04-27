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

	//for (ATogetherRunBase* actor : ChainArray) {
	//	FVector LocalFVector, JointTarget;
	//	
	//	actor->GetCharacterMovement()->MaxWalkSpeed = fSpeed;

	//	FVector NPCHaneSocketNameLocation = actor->GetMesh()->GetSocketLocation(TogetherNPCLeftHandName);

	//	// 차이 계산
	//	FVector Offset = HandLocation - NPCHaneSocketNameLocation;

	//	//FVector ExtraRightOffset = Player->GetActorRightVector() * 15.0f;
	//	//FVector ExtraBackOffset = -Player->GetActorForwardVector() * 60.0f * i;
	//	FVector ExtraRightOffset = Player->GetActorRightVector() * Offset.X;
	//	FVector ExtraBackOffset = -Player->GetActorForwardVector() * Offset.Z * i;
	//	// 
	//	//FVector ExtraUpOffset = Player->GetActorUpVector() * Offset.Z;

	//	UE_LOG(LogTemp, Error, TEXT("index : %d, OffsetZ : %f"), i, Offset.Z);

	//	// NPC 이동
	//	FVector ActorLocation = Player->GetActorLocation() + ExtraRightOffset + ExtraBackOffset;

	//	actor->SetActorLocation(ActorLocation);

	//	UTogetherRunAnimInstance* TogetherRunAnimInstance = Cast<UTogetherRunAnimInstance>(actor->GetMesh()->GetAnimInstance());

	//	TogetherRunAnimInstance->SetfTogetherAnimGroundSpeed(fSpeed);
	//	TogetherRunAnimInstance->SetfTogetherAnimShouldMove(fSpeed > 0);

	//	FVector PlayerHandLocation = HandLocation;

	//	TogetherRunAnimInstance->SetJointTarget(PlayerHandLocation);

	//	if (i == 1) {
	//		LocalFVector = Player->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName);
	//		//LocalFVector.Y += Offset.Y;

	//		TogetherRunAnimInstance->SetLocalFVector(LocalFVector);
	//	}
	//	else {
	//		LocalFVector = ChainArray[i - 2]->GetMesh()->GetSocketLocation(TogetherNPCRIghtHandName);
	//		//LocalFVector.Y += Offset.Y;

	//		TogetherRunAnimInstance->SetLocalFVector(LocalFVector);
	//	}

	//	i++;
	//}

	int32 index = 0;
	// NPC 사이의 물리적 간격 (캡슐 크기에 맞춰 조절)
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
		actor->SetActorLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
		//actor->SetActorLocation(NewLocation);

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
		AnimInst->SetJointTarget(TargetHandLocation); // 관절 꺾임 방지

		index++;
	}

}

TArray<ATogetherRunBase*> UTogetherManager::GetChainArray()
{
	return ChainArray;
}

