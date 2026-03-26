// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStat.h"
#include "ProjectPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterStat/StatAnimInstance/StatAnimInstance.h"

// Sets default values
ACharacterStat::ACharacterStat()
{
	PrimaryActorTick.bCanEverTick = true;

    // 2. 카메라 생성 및 카메라 붐 끝에 부착
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(RootComponent);

    RelicCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("RelicCamera"));
    RelicCamera->SetupAttachment(RootComponent);

    SkillCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SkillCamera"));
    SkillCamera->SetupAttachment(RootComponent);

    StarCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("StarCamera"));
    StarCamera->SetupAttachment(RootComponent);

    MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
    MainCamera->SetupAttachment(RootComponent);
    
}

// Called when the game starts or when spawned
void ACharacterStat::BeginPlay()
{
	Super::BeginPlay();
	
    TargetCam = MainCamera;

    // 시작 시점의 위치를 기본 카메라 위치로 초기화
    if (ViewCamera)
    {
        TargetLocation = ViewCamera->GetRelativeLocation();
        TargetRotation = ViewCamera->GetRelativeRotation();
    }

    RelicCamera->SetActive(false);
    MainCamera->SetActive(false);
    SkillCamera->SetActive(false);
    StarCamera->SetActive(false);
    ViewCamera->SetActive(true);

    StatAnimInstance = Cast<UStatAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void ACharacterStat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLoc = FMath::VInterpTo(ViewCamera->GetRelativeLocation(), TargetLocation, DeltaTime, CameraInterpSpeed);
    FRotator NewRot = FMath::RInterpTo(ViewCamera->GetRelativeRotation(), TargetRotation, DeltaTime, CameraInterpSpeed);

    ViewCamera->SetRelativeLocation(NewLoc);
    ViewCamera->SetRelativeRotation(NewRot);

}

// Called to bind functionality to input
void ACharacterStat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterStat::SwitchCameraComponent(ECharacterMenuState MenuState)
{

    if (CurMenuState == MenuState) return;

    switch (MenuState)
    {
    case ECharacterMenuState::Relic:
        TargetCam = RelicCamera;
        break;
    case ECharacterMenuState::Skill:
        TargetCam = SkillCamera;
        break;
    case ECharacterMenuState::Star:
        TargetCam = StarCamera;
        break;
    case ECharacterMenuState::Main:
        TargetCam = MainCamera;
        break;
    }

    CurMenuState = MenuState;

    PlayAnimation(MenuState);

    if (TargetCam)
    {
        // 목표 지점을 해당 카메라의 '상대 좌표'로 설정합니다.
        TargetLocation = TargetCam->GetRelativeLocation();
        TargetRotation = TargetCam->GetRelativeRotation();
    }
}

void ACharacterStat::ChangeCamera()
{
    AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());

    PC->SetViewTargetWithBlend(this, 0.f, VTBlend_Cubic);

    TargetCam = MainCamera;

    TargetLocation = TargetCam->GetRelativeLocation();
    TargetRotation = TargetCam->GetRelativeRotation();

    ViewCamera->SetRelativeLocation(TargetLocation);
    ViewCamera->SetRelativeRotation(TargetRotation);
}

void ACharacterStat::SetCurrentCharacterKey(FName value)
{
    CurrentCharacterKey = value;
}

FName ACharacterStat::GetCurrentCharacterKey()
{
    return CurrentCharacterKey;
}

void ACharacterStat::PlayAnimation(ECharacterMenuState value)
{
    //Y 수직 X 수평
    //1 main 정면
    //2 relic 오른쪽 위
    //3 skill 왼쪽 아래
    //4 star 왼쪽 위

    //Relic	Skill

    //Main	Star

    switch (value)
    {
    case ECharacterMenuState::Main:
        StatAnimInstance->SetBlendSpace(0, 0);
        break;
    case ECharacterMenuState::Relic:
        StatAnimInstance->SetBlendSpace(1, 0);
        break;
    case ECharacterMenuState::Skill:
        StatAnimInstance->SetBlendSpace(0, 1);
        break;
    case ECharacterMenuState::Star:
        StatAnimInstance->SetBlendSpace(1, 1);
        break;
    default:
        break;
    }

}

