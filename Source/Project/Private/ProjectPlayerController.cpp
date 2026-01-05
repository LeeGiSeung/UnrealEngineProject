// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h" //blueprint 기능을 c++에서도 사용
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Engine/PostProcessVolume.h"
#include "ProjectCharacter.h"

#include "Camera/CameraActor.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.h"

AProjectPlayerController::AProjectPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    StartTime = 0.f;
}

AProjectPlayerController::~AProjectPlayerController()
{
}

void AProjectPlayerController::StartRealTimeTimer()
{
    if (IsBlackWhite) return;
    StartTime = FPlatformTime::Seconds(); //게임 내에 구애받지 않는 진짜 시간
    IsBlackWhite = true;

    CameraGrayTrans();

    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void AProjectPlayerController::StartSpecialAttack()
{
    if (IsBlackWhite) return;

    UAnimInstance* AnimInst = PCharacter->GetMesh()->GetAnimInstance();
    UBaseAnimInstance* MyABP = Cast<UBaseAnimInstance>(AnimInst);

    if (!MyABP) {
        UE_LOG(LogTemp, Warning, TEXT("No ABP"));
        return;
    }

    //UE_LOG(LogTemp, Warning, TEXT("good"));

    MyABP->PlaySpecialAttackMontage(PCharacter);

    SpecialCameraUse();

}

void AProjectPlayerController::ScrollZomm_Down(float ActionValue)
{
    if (CurCameraDistance >= CameraDistance) return;
    CurCameraDistance += 20.f;

    FVector BaseLocation = PCamera->GetComponentLocation();
    FVector HeadLocation = PCharacter->GetMesh()->GetSocketLocation(TEXT("head"));

    FVector DirVector = (HeadLocation - BaseLocation).GetSafeNormal();

    BaseLocation += 20.f * DirVector;
    PCamera->SetWorldLocation(BaseLocation);
}

void AProjectPlayerController::ScrollZomm_Up(float ActionValue)
{
    if (CurCameraDistance <= 0) return;
    CurCameraDistance -= 20.f;

    FVector BaseLocation = PCamera->GetComponentLocation();
    FVector HeadLocation = PCharacter->GetMesh()->GetSocketLocation(TEXT("head"));

    FVector DirVector = (HeadLocation - BaseLocation).GetSafeNormal();

    BaseLocation += -20.f * DirVector;
    PCamera->SetWorldLocation(BaseLocation);
}

void AProjectPlayerController::SpecialCameraUse()
{
    if (!FaceCameraAnchor || !FaceCameraActor || !PCharacter) return;

    //시야, 이동 변경 금지
    SetIgnoreLookInput(true);
    SetIgnoreMoveInput(true);

    //삼각대의 위치를 얻는다.
    FVector CameraLocation = FaceCameraAnchor->GetComponentLocation();

    //카메라를 삼각대 위치에 놓는다.
    FaceCameraActor->SetActorLocation(CameraLocation);

    //카메라 위치에서 캐릭터의 Head 소켓으로 향하는 회전을 얻는다.
    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
        CameraLocation,
        PCharacter->GetMesh()->GetSocketLocation(TEXT("head"))
    );

    //카메라를 회전시킨다.
    FaceCameraActor->SetActorRotation(LookAtRot);

    //카메라로 시점을 변경한다.
    SetViewTargetWithBlend(
        FaceCameraActor,
        0.f,
        EViewTargetBlendFunction::VTBlend_EaseInOut
    );

    ABP_Player->OnSpecialAttackFinished.AddUObject(
        this,
        &AProjectPlayerController::ReturnToPlayerCamera
    );

}

void AProjectPlayerController::Tick(float DeltaTime) {

    Super::Tick(DeltaTime);

    if (!IsBlackWhite) return; //아직 실행 안됬으면 return;
  
    const float CurrentTime = FPlatformTime::Seconds();

    if (CurrentTime - StartTime >= TimeDuration) {

        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

        IsBlackWhite = false;

        CameraColorTrans();
    }

}

void AProjectPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    PPawn = InPawn;
    PCharacter = Cast<AProjectCharacter>(PPawn);
    if (!PCharacter) return;

    PCamera = PCharacter->FindComponentByClass<UCameraComponent>();
    if (!PCamera) return;

    //PCamera->PostProcessSettings.bOverride_ColorSaturation = true;

    TArray<AActor*> FoundVolumes;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        APostProcessVolume::StaticClass(),
        FoundVolumes
    );

    for (AActor* Actor : FoundVolumes)
    {
        if (Actor->ActorHasTag(TEXT("GrayFilter")))
        {
            PostProcessVolume = Cast<APostProcessVolume>(Actor);
            break;
        }
    }

    //# Special Camera
    SpecialCameraSetting();
}

void AProjectPlayerController::SpecialCameraSetting()
{

    AProjectCharacter* ProjectChar = Cast<AProjectCharacter>(PCharacter);
    if (!ProjectChar) return;

    FaceCameraAnchor = ProjectChar->FaceCameraAnchor;
    //FaceCameraAnchor : 삼각대
    //FaceCameraActor : 카메라
    if (FaceCameraActor) return;
    FaceCameraActor = GetWorld()->SpawnActor<ACameraActor>();

    UAnimInstance* AnimInst = PCharacter->GetMesh()->GetAnimInstance();
    ABP_Player = Cast<UBaseAnimInstance>(AnimInst);

    if (!ABP_Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("AnimInstance is not UBaseAnimInstance"));
        return;
    }
}

void AProjectPlayerController::CameraGrayTrans()
{
    //if (!PCamera) return;
    //PCamera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
    //UE_LOG(LogTemp, Warning, TEXT("Gray"));

    if (!PostProcessVolume) return;
    PostProcessVolume->BlendWeight = 1.f;
}

void AProjectPlayerController::CameraColorTrans()
{
    //if (!PCamera) return;
    //PCamera->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);
    //UE_LOG(LogTemp, Warning, TEXT("Color"));

    if (!PostProcessVolume) return;
    PostProcessVolume->BlendWeight = 0.f;
}

void AProjectPlayerController::ReturnToPlayerCamera()
{
    SetViewTargetWithBlend(
        PCharacter,
        0.f,
        EViewTargetBlendFunction::VTBlend_EaseInOut
    );

    //시야, 이동 변경 허용
    SetIgnoreLookInput(false);
    SetIgnoreMoveInput(false);
}

