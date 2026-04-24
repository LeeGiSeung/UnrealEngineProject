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

}



// Called when the game starts or when spawned
void ATogetherRunBase::BeginPlay()
{
	Super::BeginPlay();
	
	BasicSetting();

}

void ATogetherRunBase::BasicSetting()
{
	FReferenceProjectPlayer.AddUObject(this, &ATogetherRunBase::SetProjectPlayerReference);

	OnfGroundSpeedFromPlayer.AddUObject(this, &ATogetherRunBase::SetTogetherActorSpeed);

	TogetherRunAnimInstance = Cast<UTogetherRunAnimInstance>(GetMesh()->GetAnimInstance());

	TogetherRunAnimInstance->SetNPCReference(this); //NPC 주소 참조시켜줌

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ATogetherRunBase::SetProjectPlayerReference(AProjectCharacter* Player)
{
	UE_LOG(LogTemp, Error, TEXT("SetProjectPlayerReference"));

	PlayerCharacter = Player;

	GetCapsuleComponent()->IgnoreActorWhenMoving(PlayerCharacter, true);
}

// Called every frame
void ATogetherRunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCharacter) return; //PlayerCharacter가 없으면 검사 조차안됨

	SetTogetherActorTurn();
	
	float Radius = 100.0f;

	// 1. 검색할 물리 타입 지정 (이게 없으면 검색 효율이 떨어지거나 엉뚱한 게 잡힙니다)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// 2. 본인 클래스 지정
	TSubclassOf<ATogetherRunBase> TargetClass = ATogetherRunBase::StaticClass();

	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(this);

	TArray<AActor*> OutActors;

	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Red, false, 0.1f);

	bool bOverlap = UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		GetActorLocation(),
		Radius,
		ObjectTypes,
		TargetClass,
		IgnoreActor,
		OutActors
	);

	float MinDistance = FMath::Square(100);
	ATogetherRunBase* NextTogetherActor = nullptr;

	if (bOverlap && OutActors.Num() > 0) {
		for (AActor* Actor : OutActors) {
			//Actor가 Player를 잡았는지부터 확인해야함
			if (ATogetherRunBase* TestActor = Cast<ATogetherRunBase>(Actor)) { //손을 잡고있지 않는 애부터 잡아야함
				if (TestActor->GetTogetherBaseProjectCharacter() != nullptr) continue; //만약 주위 액터중에 플레이어를 이미 집고있으면 안됨
				float Dist = FVector::DistSquared(Actor->GetActorLocation(), GetActorLocation());

				if (MinDistance > Dist) {
					MinDistance = Dist;
					NextTogetherActor = TestActor;
				}
			}
		}
	}

	if (NextTogetherActor == nullptr) return;

	//여기서 나온게 최종적으로 가장 가까운 놈임
	ATogetherRunBase* ChainTogetherActor =NextTogetherActor;

	ChainTogetherActor->SetProjectPlayerReference(PlayerCharacter); //플레이어 등록해주고

	//여기서 TogetherAnimInstance에서 Together True해주고
	//ChainTogetherActor는 내 팔을 잡도록 해야함

}

void ATogetherRunBase::SetTogetherActorTurn()
{
	FVector Direction = PlayerCharacter->GetActorForwardVector();

	AddMovementInput(Direction, 1.0f);
}

// Called to bind functionality to input
void ATogetherRunBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATogetherRunBase::SetTogetherActorSpeed(float value, FVector HandLocation)
{
	fGroundSpeed = value;
	GetCharacterMovement()->MaxWalkSpeed = fGroundSpeed;

	PlayerRightHandLocation = HandLocation;

	NPCHaneSocketNameLocation = GetMesh()->GetSocketLocation(TogetherNPCLeftHandName);

	// 차이 계산
	FVector Offset = PlayerRightHandLocation - NPCHaneSocketNameLocation;

	FVector ExtraRightOffset = PlayerCharacter->GetActorRightVector() * 25.0f;
	FVector ExtraBackOffset = -PlayerCharacter->GetActorForwardVector() * 40.0f;

	float LocalX = GetActorLocation().X;
	float LocalY = GetActorLocation().Y;
	float LocalZ = GetActorLocation().Z;

	LocalX += Offset.X;
	LocalY += Offset.Y;
	// NPC 이동
	
	FVector ActorLocation = FVector(LocalX, LocalY, LocalZ) + ExtraRightOffset + ExtraBackOffset;

	SetActorLocation(ActorLocation);

	TogetherRunAnimInstance->SetfTogetherAnimGroundSpeed(fGroundSpeed);
	TogetherRunAnimInstance->SetfTogetherAnimShouldMove(fGroundSpeed > 0);

	//여기서 Player의 HandR Location, Rotation 보내줘야함

	//이걸 PlayerCharacter로 하지않고 PlyaerCharcter, TargetActorBase로 아무거나 받게하고 아래 RightHandLocation 오버라이드로 손 위치 받아서 넘기게하면됨

	check(PlayerCharacter);

	FVector PlayerHandLocation = PlayerCharacter->GetPlayerRightHandLocation();

	TogetherRunAnimInstance->SetHandLocation(PlayerHandLocation);

}

AProjectCharacter* ATogetherRunBase::GetTogetherBaseProjectCharacter()
{
	return PlayerCharacter;
}

