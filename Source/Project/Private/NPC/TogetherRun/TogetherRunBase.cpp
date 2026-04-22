// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC/TogetherRun/TogetherRunBase.h"

#include "ProjectCharacter.h"
#include "NPC/TogetherRun/AnimInstance/TogetherRunAnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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
	
	FReferenceProjectPlayer.AddUObject(this , &ATogetherRunBase::SetProjectPlayerReference);

	OnfGroundSpeedFromPlayer.AddUObject(this, &ATogetherRunBase::SetTogetherActorSpeed);

	TogetherRunAnimInstance = Cast<UTogetherRunAnimInstance>(GetMesh()->GetAnimInstance());

	TogetherRunAnimInstance->SetNPCReference(this); //NPC 주소 참조시켜줌

	GetCharacterMovement()->bOrientRotationToMovement = true;

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

void ATogetherRunBase::SetProjectPlayerReference(AProjectCharacter* Player)
{
	PlayerCharacter = Player;

	GetCapsuleComponent()->IgnoreActorWhenMoving(PlayerCharacter, true);
}

// Called every frame
void ATogetherRunBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!PlayerCharacter) return;

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

	check(PlayerCharacter);

	FVector PlayerHandLocation = PlayerCharacter->GetPlayerRightHandLocation();

	TogetherRunAnimInstance->SetHandLocation(PlayerHandLocation);

}

