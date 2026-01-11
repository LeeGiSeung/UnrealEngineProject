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
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"

#include "Engine/StaticMeshActor.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"           // GetWorld()
#include "GameFramework/Actor.h"    // AActor
#include "GameFramework/Pawn.h"     // APawn
#include "Engine/EngineTypes.h"     // FCollisionQueryParams

#include "DrawingBaseActor.h"
#include "FWidget.h"

AProjectPlayerController::AProjectPlayerController()
{
    PrimaryActorTick.bCanEverTick = true;
    StartTime = 0.f;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshFinder(
        TEXT("/Engine/BasicShapes/Cube.Cube")
    );

    if (CubeMeshFinder.Succeeded())
    {
        CubeMesh = CubeMeshFinder.Object;
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to load Cube mesh in constructor!"));
    }
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

    PCharacter->GetCharacterMovement()->StopMovementImmediately();

    //시야, 이동 변경 금지
    SetIgnoreLookInput(true);
    SetIgnoreMoveInput(true);

    UAnimInstance* AnimInst = PCharacter->GetMesh()->GetAnimInstance();
    UBaseAnimInstance* MyABP = Cast<UBaseAnimInstance>(AnimInst);

    if (!MyABP) {
        UE_LOG(LogTemp, Warning, TEXT("No ABP"));
        return;
    }

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

void AProjectPlayerController::DashStart()
{
    if (bIsDashing) return;
    if (!PCharacter) return;

    UCharacterMovementComponent* MoveComp = PCharacter->GetCharacterMovement();

    if (!MoveComp) return;

    DefaultWalkSpeed = MoveComp->MaxWalkSpeed;
    MoveComp->MaxWalkSpeed = DefaultWalkSpeed + 200.f;

    bIsDashing = true;
}

void AProjectPlayerController::DashEnd()
{
    if (!bIsDashing) return;
    if (!PCharacter) return;

    UCharacterMovementComponent* MoveComp = PCharacter->GetCharacterMovement();
        
    if (!MoveComp) return;

    MoveComp->MaxWalkSpeed = DefaultWalkSpeed;
    bIsDashing = false;
}

void AProjectPlayerController::DrawingStart()
{
    ProjectChar->bIsDrawing = true;
    bIsDrawing = true;

}

void AProjectPlayerController::DrawingEnd()
{
    ProjectChar->bIsDrawing = false;
    bIsDrawing = false;

    
}

void AProjectPlayerController::SetDrawingPosition(TArray<FVector2D> _DrawPosition)
{
    UE_LOG(LogTemp, Warning, TEXT("position Num : %d"), _DrawPosition.Num());
    DrawPosition = _DrawPosition;

    SpawnDrawingObject();
}

void AProjectPlayerController::SpawnDrawingObject()
{
    if (DrawPosition.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawPosition Zero"));
        return;
    }

    FVector2D Sum(0.f, 0.f);
    for (const FVector2D& Pos : DrawPosition)
    {
        Sum += Pos;
    }
    FVector2D Center = Sum / DrawPosition.Num();
    UE_LOG(LogTemp, Warning, TEXT("Center Position : %s"), *Center.ToString());

    FVector WorldLocation, WorldDirection;
    if (!DeprojectScreenPositionToWorld(Center.X, Center.Y, WorldLocation, WorldDirection)) //PlayerController가 소유한 카메라를 기준으로
    {
        UE_LOG(LogTemp, Warning, TEXT("DeprojectScreenPositionToWorld failed"));
        return;
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("WorldOLocation : %f"), WorldLocation);
    }

    FVector Start = WorldLocation;
    FVector End = Start + WorldDirection * 10000.f;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn()); //나 자신은 무시

    FHitResult Hit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params); //맞는지 검사

#if !UE_BUILD_SHIPPING
    DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.f, 0, 1.f); //디버그 모드에서만 line그려서 검사확인
#endif

    if (bHit && Hit.GetActor() && Hit.GetActor()->ActorHasTag(TEXT("DrawAble"))) //맞았고, 해당 위치에 액터가 있고, 액터가 그릴 수 있는 액터면
    {
        SpawnCubeAtHit(Hit); //액터 소환
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("No valid hit on Drawable actor"));
    }
}

void AProjectPlayerController::DrawingObject_UseAbility()
{
    if (!DrawingActor) {
        UE_LOG(LogTemp, Warning, TEXT("NONE DrawingActor"));
        return;
    }
    DrawingActor->UseAbility();
}

void AProjectPlayerController::RegisterDrawingActor(ADrawingBaseActor* _ADrawingBaseActor)
{
    TrackedActors.AddUnique(_ADrawingBaseActor);
    //그냥 add하면 blueprint, c++ 두번 들어감
    //UE_LOG(LogTemp, Warning, TEXT("%d"),TrackedActors.Num());
}

void AProjectPlayerController::UnregisterDrawingActor(ADrawingBaseActor* _ADrawingBaseActor)
{
    TrackedActors.Remove(_ADrawingBaseActor);
}

void AProjectPlayerController::SpawnCubeAtHit(const FHitResult& Hit)
{
    FVector CubeSpawnLocation = Hit.ImpactPoint + Hit.ImpactNormal;
    FRotator CubeSpawnRotation = FRotator::ZeroRotator;

    AStaticMeshActor* Cube = GetWorld()->SpawnActor<AStaticMeshActor>(
        AStaticMeshActor::StaticClass(), CubeSpawnLocation, CubeSpawnRotation);

    if (Cube && CubeMesh)
    {
        Cube->GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
        Cube->GetStaticMeshComponent()->SetStaticMesh(CubeMesh);
    }
    else if (!CubeMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("CubeMesh is null! Did you load it in the constructor?"));
    }
}

void AProjectPlayerController::SpecialCameraUse()
{
    if (!FaceCameraAnchor || !FaceCameraActor || !PCharacter) return;

    //삼각대의 위치를 얻는다.
    FVector CameraLocation = FaceCameraAnchor->GetComponentLocation();

    FVector PlayerLocation = PCharacter->GetActorLocation();

    //CameraLocation.X = PlayerLocation.X - 15.f;
    //FaceCameraAnchor->SetWorldLocation(CameraLocation);

    //카메라를 삼각대 위치에 놓는다.
    FaceCameraActor->SetActorLocation(CameraLocation);

    ////카메라 위치에서 캐릭터의 Head 소켓으로 향하는 회전을 얻는다.
    //FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
    //    CameraLocation,
    //    PCharacter->GetMesh()->GetSocketLocation(TEXT("head"))
    //);

    FVector LookTarget = PCharacter->GetActorLocation();
    LookTarget.Z += 80.f; // 눈높이

    FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(
        CameraLocation,
        LookTarget
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
    //Drawing Object 사용
    CheckAccTime += DeltaTime;
    if (CheckAccTime >= CheckInterval) {
        CheckAccTime = 0.f;

        for (int32 i = 0; i < TrackedActors.Num(); i++)
        {
            if (!TrackedActors[i].IsValid())
            {
                TrackedActors.RemoveAt(i); //혹시 모르니까 여기서 삭제
                continue;
            }

            //
            DrawingActor = TrackedActors[i].Get();

            if (!DrawingActor) continue;

            UStaticMeshComponent* MeshComp = DrawingActor->FindComponentByClass<UStaticMeshComponent>();

            if (!MeshComp) UE_LOG(LogTemp, Warning, TEXT("No UStaticMeshComponent"));

            bool bVisible = MeshComp && MeshComp->WasRecentlyRendered(0.1f); //UPrimitiveComponent에서만 가능한 함수 이번에 그려졌냐를 검사

            if (!InteractWidget) {
                //UE_LOG(LogTemp, Warning, TEXT("NO WIDGET"));
                continue;
            }

            if (bVisible)
            {
                bFindObject = true;
                FVector WorldLocation = DrawingActor->GetActorLocation();
                FVector2D ScreenPos;

                //UE_LOG(LogTemp, Warning, TEXT("VISIBLE"));

                if (ProjectWorldLocationToScreen(WorldLocation, ScreenPos))
                {
                    InteractWidget->SetPositionInViewport(ScreenPos, true);
                    InteractWidget->SetVisibility(ESlateVisibility::Visible);
                    bFindObject = true;

                    //UE_LOG(LogTemp, Warning, TEXT("SEE WIDGET"));

                    break;
                }
            }
            else
            {
                bFindObject = false;
                InteractWidget->SetVisibility(ESlateVisibility::Hidden);
                //UE_LOG(LogTemp, Warning, TEXT("NO OBJECT"));
            }
        }
    }
    
    //흑백 상태
    if (IsBlackWhite) {
        const float CurrentTime = FPlatformTime::Seconds();

        if (CurrentTime - StartTime >= TimeDuration) {

            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

            IsBlackWhite = false;

            CameraColorTrans();

            OnActionTriggered.Broadcast();
        }
    }
}

void AProjectPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    PPawn = InPawn;
    PCharacter = Cast<ACharacter>(PPawn);
    if (!PCharacter) return;

    ProjectChar = Cast<AProjectCharacter>(PCharacter);
    if (!ProjectChar) return;

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


    TArray<AActor*> Found;
    UGameplayStatics::GetAllActorsOfClass(
        GetWorld(),
        ADrawingBaseActor::StaticClass(),
        Found
    );

    for (AActor* A : Found)
    {
        TrackedActors.Add(Cast<ADrawingBaseActor>(A));
    }

    InteractWidget = CreateWidget<UFWidget>(this, InteractWidgetClass);
    if (InteractWidget)
    {
        InteractWidget->AddToViewport();
        InteractWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AProjectPlayerController::SpecialCameraSetting()
{

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

