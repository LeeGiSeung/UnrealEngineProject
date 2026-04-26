// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/TogetherRun/TogetherRunBase.h"

#include "ProjectCharacter.h"
#include "NPC/TogetherRun/AnimInstance/TogetherRunAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values
ATogetherRunBase::ATogetherRunBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 혹은 ECR_Block
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

}

void ATogetherRunBase::BeginPlay()
{
	Super::BeginPlay();
	
	BasicSetting();

}

void ATogetherRunBase::BasicSetting()
{

	TogetherRunAnimInstance = Cast<UTogetherRunAnimInstance>(GetMesh()->GetAnimInstance());
	TogetherRunAnimInstance->SetNPCReference(this); //NPC 주소 참조시켜줌
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// 1. 물리와 감지 모두 켭니다.
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	// 2. NPC의 정체성(Object Type)을 확실하게 'Pawn'으로 강제합니다.
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);

	// 3. 중요: NPC 입장에서 다가오는 Player의 구체(WorldDynamic)를 허용(Overlap)해 줍니다.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	// 4. 오버랩 이벤트를 켭니다.
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);

	// 2. 그 중 Pawn(플레이어의 감지 구체)에 대해서만 Overlap으로 설정
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 3. 물리 상태 확인
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ATogetherRunBase::SetFrontActorReference(AProjectCharacter* value)
{
	Player = value;

	GetCapsuleComponent()->IgnoreActorWhenMoving(value, true);
}

// Called every frame
void ATogetherRunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player) return; //PlayerCharacter가 없으면 검사 조차안됨

	SetTogetherActorTurn();

	//if (ChainTogetherActor) {
	//	ChainTogetherActor->SetTogetherActorSpeed(this, fGroundSpeed, GetMesh()->GetSocketLocation(TogetherNPCLeftHandName));
	//	return;
	//}

	//float Radius = 100.0f;

	//// 1. 검색할 물리 타입 지정 (이게 없으면 검색 효율이 떨어지거나 엉뚱한 게 잡힙니다)
	//TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	//ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	//// 2. 본인 클래스 지정
	//TSubclassOf<ATogetherRunBase> TargetClass = ATogetherRunBase::StaticClass();

	//TArray<AActor*> IgnoreActor;
	//IgnoreActor.Add(this);

	//TArray<AActor*> OutActors;

	////DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Red, false, 0.1f);

	//bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(
	//	GetWorld(),
	//	GetActorLocation(),
	//	Radius,
	//	ObjectTypes,
	//	TargetClass,
	//	IgnoreActor,
	//	OutActors
	//);

	//float MinDistance = FMath::Square(100);
	//ATogetherRunBase* NextTogetherActor = nullptr;

	//if (bOverlap && OutActors.Num() > 0) {
	//	for (AActor* Actor : OutActors) {
	//		//Actor가 Player를 잡았는지부터 확인해야함
	//		if (ATogetherRunBase* TestActor = Cast<ATogetherRunBase>(Actor)) { //손을 잡고있지 않는 애부터 잡아야함
	//			if (TestActor->GetTogetherBaseLeaderObject() != nullptr || TestActor->ChainTogetherActor != nullptr) continue; //만약 주위 액터중에 플레이어를 이미 집고있으면 안됨
	//			float Dist = FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation());

	//			if (MinDistance > Dist) {
	//				MinDistance = Dist;
	//				NextTogetherActor = TestActor;
	//			}
	//		}
	//	}
	//}

	//if (NextTogetherActor == nullptr) return;

	////여기서 나온게 최종적으로 가장 가까운 놈임
	//ChainTogetherActor = NextTogetherActor;

	//ChainTogetherActor->SetFrontActorReference(Leader); //플레이어 등록해주고

	//TogetherRunAnimInstance->SetbTogetherChild(true);

}

void ATogetherRunBase::SetTogetherActorTurn()
{
	FVector Direction = Player->GetActorForwardVector();

	AddMovementInput(Direction, 1.0f);
}

// Called to bind functionality to input
void ATogetherRunBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATogetherRunBase::SetTogetherActorSpeed(AActor* FrontActor, float value, FVector HandLocation)
{

	//Leader = FrontActor;

	//check(Leader);
	//fGroundSpeed = value;
	//GetCharacterMovement()->MaxWalkSpeed = fGroundSpeed;

	//PlayerRightHandLocation = HandLocation;

	//NPCHaneSocketNameLocation = GetMesh()->GetSocketLocation(TogetherNPCLeftHandName);

	//// 차이 계산
	//FVector Offset = PlayerRightHandLocation - NPCHaneSocketNameLocation;

	//FVector ExtraRightOffset = Leader->GetActorRightVector() * 10.0f;
	//FVector ExtraBackOffset = -Leader->GetActorForwardVector() * 40.0f;

	//float LocalX = GetActorLocation().X;
	//float LocalY = GetActorLocation().Y;
	//float LocalZ = GetActorLocation().Z;

	//LocalX += Offset.X;
	//LocalY += Offset.Y;
	//// NPC 이동
	//
	//FVector ActorLocation = FVector(LocalX, LocalY, LocalZ) + ExtraRightOffset + ExtraBackOffset;

	//SetActorLocation(ActorLocation);

	//TogetherRunAnimInstance->SetfTogetherAnimGroundSpeed(fGroundSpeed);
	//TogetherRunAnimInstance->SetfTogetherAnimShouldMove(fGroundSpeed > 0);

	////여기서 Player의 HandR Location, Rotation 보내줘야함

	////이걸 PlayerCharacter로 하지않고 PlyaerCharcter, TargetActorBase로 아무거나 받게하고 아래 RightHandLocation 오버라이드로 손 위치 받아서 넘기게하면됨

	//FVector PlayerHandLocation = HandLocation;

	//TogetherRunAnimInstance->SetHandLocation(PlayerHandLocation);

}

AActor* ATogetherRunBase::GetTogetherBaseLeaderObject()
{
	return Player;
}

void ATogetherRunBase::SetChainIndex(int value)
{
	chainIndex = value;
}

int ATogetherRunBase::GetChainIndex()
{
	return chainIndex;
}

