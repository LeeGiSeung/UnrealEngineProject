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
#include "Engine/DecalActor.h"
#include "Components/DecalComponent.h"
#include "DrawingActor/Drawing_Decal_Actor.h"
#include "DrawDebugHelpers.h"

#include "Energy/EnergyWidget.h"


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

    fDrawingEnergy = 100.f;
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

void AProjectPlayerController::SpawnDecalActor(TArray<FVector2D> _DrawPosition, EColor CurChoiceColor)
{
    DrawPosition = _DrawPosition;

    FVector2D MinScale = FVector2D(FLT_MAX, FLT_MAX);
    FVector2D MaxScale = FVector2D(-FLT_MAX, -FLT_MAX);

    for (const FVector2D& P : _DrawPosition)
    {
        MinScale.X = FMath::Min(MinScale.X, P.X);
        MinScale.Y = FMath::Min(MinScale.Y, P.Y);

        MaxScale.X = FMath::Max(MaxScale.X, P.X);
        MaxScale.Y = FMath::Max(MaxScale.Y, P.Y);
    }

    FVector2D Size = MaxScale - MinScale;
    UE_LOG(LogTemp, Warning, TEXT("Size 크기 : %s"), *Size.ToString());
    float BaseWidth = 100.f;
    float BaseHeight = 100.f;

    float ScaleX = Size.X / BaseWidth;
    float ScaleY = Size.Y / BaseHeight;

    SetActorSpawnScale(ScaleX, ScaleY);

    DrawingColor = CurChoiceColor;

    if (DrawPosition.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("DrawPosition Zero"));
        return;
    }

    FVector2D Sum(0.f, 0.f);

    for (const FVector2D& Pos : DrawPosition) Sum += Pos;

    FVector2D Center = Sum / DrawPosition.Num();

    FVector WorldLocation, WorldDirection;
    if (!DeprojectScreenPositionToWorld(Center.X, Center.Y, WorldLocation, WorldDirection))
    {
        return;
    }

    FVector Start = WorldLocation;
    FVector End = Start + WorldDirection * 10000.f;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetPawn()); //나 자신은 무시

    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params); //맞는지 검사

#if !UE_BUILD_SHIPPING
    //DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2.f, 0, 1.f); //디버그 모드에서만 line그려서 검사확인
#endif

    if (bHit && Hit.GetActor() && Hit.GetActor()->ActorHasTag(TEXT("DrawAble"))) //맞았고, 해당 위치에 액터가 있고, 액터가 그릴 수 있는 액터면
    {
        SpawnDecalAtHit();
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
    DrawingActor->GetDecalActor()->Destroy();
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

void AProjectPlayerController::RegisterDrawingDecar(ADrawing_Decal_Actor* _ADrawingBaseDecar)
{
    TrackedDecalActors.AddUnique(_ADrawingBaseDecar);
}

void AProjectPlayerController::UnregisterDrawingDecar(ADrawing_Decal_Actor* _ADrawingBaseDecar)
{
    TrackedDecalActors.Remove(_ADrawingBaseDecar);
}

FVector AProjectPlayerController::GetActorSpawnScale()
{
    FVector result = FVector(ActorSpawnScaleX, ActorSpawnScaleY, FMath::Min(ActorSpawnScaleX, ActorSpawnScaleY));

    return result;
}

void AProjectPlayerController::SetActorSpawnScale(float _ActorSpawnScaleX, float _ActorSpawnScaleY)
{
    ActorSpawnScaleX = _ActorSpawnScaleX;
    ActorSpawnScaleY = _ActorSpawnScaleY;

    float limit = 5;

    if (ActorSpawnScaleY > limit) ActorSpawnScaleX = limit;
    if (ActorSpawnScaleY > limit) ActorSpawnScaleY = limit;
}

FHitResult AProjectPlayerController::GetHit()
{
    return Hit;
}

void AProjectPlayerController::SpawnCubeAtHit()
{

    FVector Normal = Hit.ImpactNormal;

    constexpr float SurfaceThreshold = 0.7f;

    // 표면 법선 기준 회전
    FRotator SelectedSpawnRotation = UKismetMathLibrary::MakeRotFromZ(Normal);

    FVector SelectedSpawnLocation = Hit.ImpactPoint + Normal * 2.f;

    if (SpawnActorMap.Num() == 0) {
        UE_LOG(LogTemp, Warning, TEXT("SpawnActorClasses is 0"));
        return;
    }
    
    TSubclassOf<AActor> SelectedClass = SpawnActorMap[DrawingColor];
    if (!SelectedClass) {
        UE_LOG(LogTemp, Warning, TEXT("NONE SpawnActorClass"));
        return;
    }
    
    AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(
        SelectedClass, 
        SelectedSpawnLocation,
        SelectedSpawnRotation);

    //유동적 사이즈로 변경
    if (SpawnActor) {
        SpawnActor->SetActorScale3D(GetActorSpawnScale());

        FVector Origin;
        FVector BoxExtent;

        SpawnActor->GetActorBounds(
            true, 
            Origin,
            BoxExtent
        );

        UE_LOG(LogTemp, Warning, TEXT("%s"), *BoxExtent.ToString());

        ActorXSize = BoxExtent.X;

        if (Decal) {
            FVector f = GetActorSpawnScale();
            f.X = f.X / 2.f;
            Decal->SetActorScale3D(f);

            FVector DecalCurLocation = Decal->GetActorLocation();
            UE_LOG(LogTemp, Warning, TEXT("%s"), *DecalCurLocation.ToString());
            DecalCurLocation -= Decal->GetActorForwardVector() * ActorXSize;
            UE_LOG(LogTemp, Warning, TEXT("%s"), *DecalCurLocation.ToString());

            Decal->SetActorLocation(DecalCurLocation);
        }

        //ActorYSize = BoxExtent.Y;
        //ActorZSize = BoxExtent.Z;

        //UE_LOG(LogTemp, Warning, TEXT("%f"), ActorXSize);
        //UE_LOG(LogTemp, Warning, TEXT("%f"), ActorYSize);
        //UE_LOG(LogTemp, Warning, TEXT("%f"), ActorZSize);

        ADrawingBaseActor* DrawingSpawnActor = Cast<ADrawingBaseActor>(SpawnActor);

        DrawingSpawnActor->SetDecalActor(Decal);
        
        TArray<UActorComponent*> Components;
        SpawnActor->GetComponents(UPrimitiveComponent::StaticClass(), Components);

        for (UActorComponent *com : Components) {
            if (UPrimitiveComponent* primcom = Cast<UPrimitiveComponent>(com)) {
                primcom->SetRenderCustomDepth(true);
                
                switch (DrawingColor)
                {
                case EColor::RED:
                    primcom->SetCustomDepthStencilValue(1);
                    break;
                case EColor::BLUE:
                    primcom->SetCustomDepthStencilValue(4);
                    break;
                case EColor::YELLOW:
                    primcom->SetCustomDepthStencilValue(2);
                    break;
                case EColor::GREEN:
                    primcom->SetCustomDepthStencilValue(3);
                    break;
                default:
                    break;
                }
                
            }
        }

    }

}

void AProjectPlayerController::SpawnDecalAtHit()
{
    if (!DecalMaterialMap[DrawingColor]) return;

    // 1. Slightly offset the location to avoid z-fighting
    float Margin = 2.f;
    FVector SelectedSpawnLocation = Hit.ImpactPoint + Hit.ImpactNormal;

    // 2. Base rotation: Forward aligned to impact normal
    FRotator BaseRotation = UKismetMathLibrary::MakeRotFromZ(Hit.ImpactNormal);

    // 4. Spawn decal actor
    Decal = GetWorld()->SpawnActor<ADecalActor>(SelectedSpawnLocation, BaseRotation);
    if (!Decal) return;

    // 5. Set decal material and size
    Decal->SetDecalMaterial(DecalMaterialMap[DrawingColor]);

    //Decal->GetDecal()->DecalSize = FVector(); //나중에 유동적 사이즈로 변경 //어차피 아래에서 사이즈 변경해서 여기서 설정할 필요는 업승ㅁ

    // 6. Optional lifespan
    Decal->SetLifeSpan(0.f);

    // 7. Random rotation around decal forward axis (impact normal)
    const float RandomAngle = FMath::FRandRange(0.f, 360.f);
    const FVector RotationAxis = Hit.ImpactNormal;

    FQuat RandomQuat(RotationAxis, FMath::DegreesToRadians(RandomAngle));

    Decal->AddActorWorldRotation(RandomQuat);

    //if (Decal) {
    //    FVector f = GetActorSpawnScale();
    //    f.X = f.X / 2.f;
    //    Decal->SetActorScale3D(f);

    //    FVector DecalCurLocation = Decal->GetActorLocation();
    //    UE_LOG(LogTemp, Warning, TEXT("%s"), *DecalCurLocation.ToString());
    //    DecalCurLocation += Decal->GetActorForwardVector() * ActorXSize;
    //    UE_LOG(LogTemp, Warning, TEXT("%s"), *DecalCurLocation.ToString());

    //    Decal->SetActorLocation(DecalCurLocation);
    //}

    // World 기준 회전 추가

    SpawnCubeAtHit();
}


void AProjectPlayerController::SpecialCameraUse()
{
    if (!FaceCameraAnchor || !FaceCameraActor || !PCharacter) return;

    //삼각대의 위치를 얻는다.
    FVector CameraLocation = FaceCameraAnchor->GetComponentLocation();

    FVector PlayerLocation = PCharacter->GetActorLocation();

    //카메라를 삼각대 위치에 놓는다.
    FaceCameraActor->SetActorLocation(CameraLocation);

    ////카메라 위치에서 캐릭터의 Head 소켓으로 향하는 회전을 얻는다.


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
    //Drawing Object FWidget
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

            FVector Start = PCamera->GetComponentLocation();
            FVector End = Start + PCamera->GetForwardVector() * ViewDistance;

            FHitResult DrawingObjectHit;
            FCollisionQueryParams Params;
            Params.AddIgnoredActor(this);

            bool bHit = GetWorld()->LineTraceSingleByChannel(
                DrawingObjectHit,
                Start,
                End,
                ECC_Visibility,
                Params
            );

            DrawingActor = Cast<ADrawingBaseActor>(DrawingObjectHit.GetActor());

            if (!DrawingActor) continue;

            //Decar과 CubeActor는 따로임 이 두개를 묶어서 진행해야함

            if (DrawingActor)
            {
                //UE_LOG(LogTemp,Warning,TEXT("Drawing Actor Name : %s"), *DrawingActor->GetName());
                bFindObject = true;
            }

            if (bHit)
            {
                //DrawDebugLine(
                //    GetWorld(),
                //    Start,
                //    DrawingObjectHit.ImpactPoint,
                //    FColor::Green,
                //    false,
                //    1.f,
                //    0,
                //    2.f
                //);

                DrawDebugPoint(
                    GetWorld(),
                    DrawingObjectHit.ImpactPoint,
                    10.f,
                    FColor::White,
                    false,
                    1.f
                );
            }

        }
    }
    
    if (!DrawingActor) bFindObject = false;

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