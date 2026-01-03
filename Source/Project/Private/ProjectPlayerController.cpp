// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "ProjectCharacter.h"

AProjectPlayerController::AProjectPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;

    //PPawn = GetPawn();
    //if (PPawn == NULL) {
    //    UE_LOG(LogTemp, Log, TEXT("Pawn Null"));
    //    return;
    //}

    //PCharacter = Cast<AProjectCharacter>(PPawn);
    //if (PCharacter == NULL) {
    //    UE_LOG(LogTemp, Log, TEXT("Character Null"));
    //    return;
    //}

    //PCamera = PCharacter->FindComponentByClass<UCameraComponent>();
    //if (PCamera == NULL) {
    //    UE_LOG(LogTemp, Log, TEXT("Camera Null"));
    //    return;
    //}

    //PCamera->PostProcessSettings.bOverride_ColorSaturation = true;
}

AProjectPlayerController::~AProjectPlayerController()
{
}

void AProjectPlayerController::StartRealTimeTimer()
{
   
    StartTime = FPlatformTime::Seconds(); //게임 내에 구애받지 않는 진짜 시간
    IsBlackWhite = true;

    CameraGrayTrans();

    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void AProjectPlayerController::Tick(float DeltaTime) {

    Super::Tick(DeltaTime);

    if (!IsBlackWhite) return; //아직 실행 안됬으면 return;
  
    const float CurrentTime = FPlatformTime::Seconds();

    if (CurrentTime - StartTime >= TimeDuration) {

        UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

        IsBlackWhite = false;

        if (!PCamera) return;
        PCamera->PostProcessSettings.ColorSaturation = FVector4(1, 1, 1, 1);
    }

}

void AProjectPlayerController::CameraGrayTrans()
{
    PCamera->PostProcessSettings.ColorSaturation = FVector4(0, 0, 0, 1);
}

void AProjectPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    PPawn = InPawn;
    PCharacter = Cast<AProjectCharacter>(PPawn);
    if (!PCharacter) return;

    PCamera = PCharacter->FindComponentByClass<UCameraComponent>();
    if (!PCamera) return;

    PCamera->PostProcessSettings.bOverride_ColorSaturation = true;
}

