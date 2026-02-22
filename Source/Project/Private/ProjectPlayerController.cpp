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
#include "Manager/DrawingActorManager.h"

//Energy
#include "Energy/EnergyWidget.h"
#include "Components/ProgressBar.h"
#include "EngineUtils.h"

//Cable
#include "Cable/BP_CablePouch.h"

//DialogueManager
#include "DialogueManager/DialogueManager.h"
#include "BaseUserWidget.h"

//Model
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "NNERuntimeGPU.h"

//PNG
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "ImageUtils.h"
#include "ImageCore.h"

using namespace UE::NNE;

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

    fDefaultDrawingEnergy = 10.f;
    fDrawingEnergy = fDefaultDrawingEnergy;
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

    DrawingManager->ChangeRandomActorDecalBlack();

    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}

void AProjectPlayerController::StartSpecialAttack()
{
    if (IsBlackWhite) return;

    PCharacter->GetCharacterMovement()->StopMovementImmediately();

    //시야, 이동 변경 금지
    SetIgnoreLookInput(true);
    SetIgnoreMoveInput(true);

    //UAnimInstance* AnimInst = PCharacter->GetMesh()->GetAnimInstance();
    //UBaseAnimInstance* MyABP = Cast<UBaseAnimInstance>(AnimInst);

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

    FString ImagePath = TEXT("C:/Users/82103/Documents/Unreal Projects/ProjectV/UnrealEngineProject/Content/DrawingImage/Capture.png");
        
    TArray<float> InputFeature;
    
    float Threshold = 0.85f;
    //코사인 검사
    bool bIsCorrect = IsSameShape(
        ImagePath,
        Threshold);

    if (bIsCorrect)
    {
        UE_LOG(LogTemp, Warning, TEXT("yes!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("no!"));
        return;
    }


    //제대로 그렸는지 OXNN 검사

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

    float BaseWidth = 100.f;
    float BaseHeight = 100.f;

    float ScaleX = Size.X / BaseWidth;
    float ScaleY = Size.Y / BaseHeight;

    float UseEnergy = ScaleY + ScaleY;

    
    if (CheckDrawingEnergyIsEnough(UseEnergy)) {
        UpdateEnergy(-UseEnergy);
    }
    else {
        SetSpawnRandom(true);
        //return; //DrawingEnergy가 부족하면 그림을 그릴 수 없음
    }
    
    SetActorSpawnScale(ScaleX, ScaleY);

    //DrawingColor = CurChoiceColor;

    if (GetSpawnRandom()) {
        DrawingColor = static_cast<EColor>(FMath::RandRange(0, 3));
    }
    else {
        DrawingColor = CurChoiceColor;
    }

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

void AProjectPlayerController::SpawnRandomActor()
{
    int32 RandValue = FMath::RandRange(0, 4);

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

    SpawnCubeAtHit();
}

void AProjectPlayerController::DrawingObject_UseAbility()
{

    if (!DrawingActor) {
        UE_LOG(LogTemp, Warning, TEXT("NONE DrawingActor"));
        return;
    }

    DrawingActor->UseAbility();
    DrawingActor->GetDecalActor()->Destroy(); //삭제
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

bool AProjectPlayerController::CheckDrawingEnergyIsEnough(float _Scale)
{
    return fDrawingEnergy >= _Scale;
}

void AProjectPlayerController::UpdateEnergy(float _Energy)
{
    if (fDefaultDrawingEnergy - _Energy < 0.f) return;

    fDrawingEnergy += _Energy;
    
    if (fDrawingEnergy > fDefaultDrawingEnergy) fDrawingEnergy = fDefaultDrawingEnergy;

    SetEnergyPercentGage();
}

void AProjectPlayerController::SetEnergyPercentGage()
{

    float EnergyPercent = FMath::Clamp(fDrawingEnergy / fDefaultDrawingEnergy, 0.f, 1.f);

    DrawingEnergyWidget->EnergyBar->SetPercent(EnergyPercent);

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

    SpawnCubeAtHit();
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

        ActorXSize = BoxExtent.X;

        if (Decal) {
            FVector f = GetActorSpawnScale();
            f.X = f.X / 2.f;
            Decal->SetActorScale3D(f);

            FVector DecalCurLocation = Decal->GetActorLocation();
            DecalCurLocation -= Decal->GetActorForwardVector() * ActorXSize;

            Decal->SetActorLocation(DecalCurLocation);
        }

        ADrawingBaseActor* DrawingSpawnActor = Cast<ADrawingBaseActor>(SpawnActor);

        DrawingSpawnActor->SetDecalActor(Decal);



        TArray<UActorComponent*> Components;
        SpawnActor->GetComponents(UPrimitiveComponent::StaticClass(), Components);

        if (GetSpawnRandom()) {
            DrawingSpawnActor->SetRandom(true); //랜덤 액터 표시
        }

        DrawingManager->AddDrawingActor(DrawingSpawnActor);

        DrawingSpawnActor->SetDcalMaterial(DecalMaterialMap[DrawingColor]);

        for (UActorComponent* com : Components) {
            if (UPrimitiveComponent* primcom = Cast<UPrimitiveComponent>(com)) {
                primcom->SetRenderCustomDepth(true);

                if (GetSpawnRandom()) {
                    primcom->SetCustomDepthStencilValue(6);
                    continue;
                }

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

    SetSpawnRandom(false);
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

    if (fDrawingEnergy < fDefaultDrawingEnergy) {
        UpdateEnergy(fEnergyCharge * DeltaTime);
    }

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

            DrawingManager->ChangeRandomActorOriginalColor();

            UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);

            IsBlackWhite = false;

            CameraColorTrans();

            OnActionTriggered.Broadcast();

            SetMouseLocation(fCanvasSizeWidth / 2, fCanvasSizeHeight / 2);
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

void AProjectPlayerController::BeginPlay() {

    Super::BeginPlay();

    DrawingManager = nullptr;

    for (TActorIterator<ADrawingActorManager> It(GetWorld()); It; ++It)
    {
        DrawingManager = *It;
        break;
    }

    if (!ModelData)
    {
        UE_LOG(LogTemp, Error, TEXT("ModelData is null"));
        return;
    }

    auto Runtime = UE::NNE::GetRuntime<INNERuntimeCPU>(TEXT("NNERuntimeORTCpu"));
    if (!Runtime.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get Runtime"));
        return;
    }

    Model = Runtime->CreateModel(ModelData);

    if (!Model) {
        UE_LOG(LogTemp, Error, TEXT("Model"));
        return;
    }

    ModelInstance = Model->CreateModelInstance();
    if (!ModelInstance.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create Model Instance"));
        return;
    }

    // 1. 입력 텐서 디스크립터 가져오기
    TConstArrayView<UE::NNE::FTensorDesc> InputTensorDescs = ModelInstance->GetInputTensorDescs();
    if (InputTensorDescs.Num() != 1)
    {
        UE_LOG(LogTemp, Error, TEXT("Unexpected number of inputs: %d"), InputTensorDescs.Num());
        return;
    }

    // 2. 심볼릭 입력 shape 가져오기
    UE::NNE::FSymbolicTensorShape SymbolicShape = InputTensorDescs[0].GetShape();

    // 3. 심볼릭 차원에서 -1 부분을 실제 크기로 치환 (예: 224,224)
    TConstArrayView<int32> SymbolicDims = SymbolicShape.GetData();

    TArray<uint32> ConcreteDims;
    ConcreteDims.Reserve(SymbolicDims.Num());

    for (int32 Dim : SymbolicDims)
    {
        int32 ActualDim = Dim;
        if (ActualDim == -1)
        {
            // 동적 차원 실제 크기 할당 예시
            ActualDim = 224;
        }
        check(ActualDim >= 0);
        ConcreteDims.Add(static_cast<uint32>(ActualDim));
    }

    // 4. Concrete tensor shape 생성
    TArray<UE::NNE::FTensorShape> InputShapes = { UE::NNE::FTensorShape::MakeFromSymbolic(SymbolicShape) };

    // 5. SetInputTensorShapes 호출 및 상태 점검
    ModelInstance->SetInputTensorShapes(InputShapes);

    if (!RunONNX(
        TEXT("C:/Users/82103/Documents/Unreal Projects/ProjectV/UnrealEngineProject/Content/DrawingImage/CaptureImg6.png"),
        CorrectFeature))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to generate CorrectFeature")); //여기 오류
        return;
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("Success Onnx"));
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
}

void AProjectPlayerController::CameraGrayTrans()
{

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
    AllowLookMove();
}

void AProjectPlayerController::AllowLookMove()
{
    if (!IsLookInputIgnored()) return;

    SetInputMode(FInputModeGameOnly());

    bShowMouseCursor = false;
    bEnableClickEvents = false;
    bEnableMouseOverEvents = false;

    SetIgnoreLookInput(false);
    SetIgnoreMoveInput(false);

    FSlateApplication::Get().SetAllUserFocusToGameViewport();
}

void AProjectPlayerController::IgnoreLookMove()
{
    if (IsLookInputIgnored()) return;

    SetIgnoreLookInput(true);
    SetIgnoreMoveInput(true);
}

void AProjectPlayerController::UseCable() {

    if (GetUseCablePouch() && CurUsePouch) { //이미 사용중이면 그냥 풀면됨
        return;
    }

    TArray<FOverlapResult> Overlaps;

    float PouchSphere = 150.f;

    bool bHit = GetWorld()->OverlapMultiByChannel(
        Overlaps,
        ProjectChar->GetActorLocation(),
        FQuat::Identity,
        ECC_WorldDynamic,
        FCollisionShape::MakeSphere(PouchSphere)
    );

    for (const FOverlapResult& result : Overlaps) {
        AActor* actor = result.GetActor();
        if (!actor) return;

        if (ABP_CablePouch* CablePouch = Cast<ABP_CablePouch>(actor)) {
            CurUsePouch = CablePouch;
            PCharacter->bUseControllerRotationYaw = true;
            CurUsePouch->UsePouch();
            SetUseCablePouch(true);
            ProjectChar->GetCharacterMovement()->MaxWalkSpeed = 100;
            break;
        }
    }
}

void AProjectPlayerController::UsePouchFly() {
    if (!GetUseCablePouch()) return; //파우치 사용중 아니면 return;

    CurUsePouch->FlyPlayer();
}

void AProjectPlayerController::UnUseCable()
{
    PCharacter->bUseControllerRotationYaw = false;
    SetUseCablePouch(false);
    ProjectChar->GetCharacterMovement()->MaxWalkSpeed = 500;
}

void AProjectPlayerController::SetCursorWidget(UBaseUserWidget* widget)
{
    if (!DialogueManager) {
        for (TActorIterator<ADialogueManager> It(GetWorld()); It; ++It)
        {
            DialogueManager = *It;
            break;
        }
    }

    CursorWidget = widget;
    DialogueManager->SetCursorWidget(CursorWidget);
}

void AProjectPlayerController::SetEnergyWidget(UEnergyWidget* widget)
{
    if (!DialogueManager) {
        for (TActorIterator<ADialogueManager> It(GetWorld()); It; ++It)
        {
            DialogueManager = *It;
            break;
        }
    }

    EnergyWidget = widget;
    DialogueManager->SetEnergyWidget(EnergyWidget);
}


void AProjectPlayerController::StartCrouchBack()
{
    //PCharacter->GetCharacterMovement()->StopMovementImmediately();

    //시야, 이동 변경 금지
    SetIgnoreLookInput(true);
    SetIgnoreMoveInput(true);

    if (!MyABP) {
        UE_LOG(LogTemp, Warning, TEXT("No ABP"));
        return;
    }

    MyABP->PlayCrouchBackMontage(PCharacter);
}

void AProjectPlayerController::ResetDialogueActor()
{
    diaActor = nullptr;
}

void AProjectPlayerController::StartDialogue() {

    if (diaActor) {
        diaActor->StartDialogue();
    }
    else {
        TArray<FOverlapResult> Overlaps;

        float fOverlapSize = 150.f;

        bool bHit = GetWorld()->OverlapMultiByChannel(
            Overlaps,
            ProjectChar->GetActorLocation(),
            FQuat::Identity,
            ECC_WorldDynamic,
            FCollisionShape::MakeSphere(fOverlapSize)
        );

        for (const FOverlapResult& result : Overlaps) {
            AActor* actor = result.GetActor();

            if (ADialogueBaseActor *TestAcftor = Cast<ADialogueBaseActor>(actor)) {
                diaActor = TestAcftor;
                if (diaActor) diaActor->StartDialogue();
                break;
            }
        }
    }

    return;

}


float AProjectPlayerController::CosineSimilarity(const TArray<float>& A, const TArray<float>& B)
{
    if (A.Num() != B.Num()) return 0.f;

    float Dot = 0.f;
    float NormA = 0.f;
    float NormB = 0.f;

    for (int32 i = 0; i < A.Num(); i++)
    {
        Dot += A[i] * B[i];
        NormA += A[i] * A[i];
        NormB += B[i] * B[i];
    }

    float Denom = FMath::Sqrt(NormA) * FMath::Sqrt(NormB);
    if (Denom <= KINDA_SMALL_NUMBER)
        return 0.f;

    return Dot / Denom;
}

bool AProjectPlayerController::IsSameShape( const FString& PlayerImagePath, float Threshold)
{
    TArray<float> PlayerFeature;

    if (!RunONNX(PlayerImagePath, PlayerFeature))
        return false;

    UE_LOG(LogTemp, Warning, TEXT("PlayerFeature Num: %d"), PlayerFeature.Num());

    float Similarity = CosineSimilarity(PlayerFeature, CorrectFeature);

    UE_LOG(LogTemp, Warning, TEXT("Similarity: %f"), Similarity);

    return Similarity > Threshold;
}

bool AProjectPlayerController::RunONNX(const FString& ImagePath, TArray<float>& OutFeature)
{
    TArray<float> InputTensor;
    if (!LoadPNG(ImagePath, InputTensor))
        return false;

    // 입력 바인딩
    TArray<FTensorBindingCPU> InputBindings;
    InputBindings.SetNum(1);

    InputBindings[0].Data = InputTensor.GetData();
    InputBindings[0].SizeInBytes = InputTensor.Num() * sizeof(float);

    // -------------------------
    // 출력 텐서 정보 가져오기
    // -------------------------
  
    TConstArrayView<UE::NNE::FTensorShape> OutputShapes = ModelInstance->GetOutputTensorShapes();
    if (OutputShapes.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("No output tensor shapes available"));
        return false;
    }

    const UE::NNE::FTensorShape& OutputShape = OutputShapes[0];

    // 2. 차원 값 유효성 검사 및 요소 수 계산
    int64 NumElements = 1;
    for (int32 Dim : OutputShape.GetData())
    {
        if (Dim <= 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid output dimension size: %d"), Dim);
            return false;
        }
        NumElements *= Dim;
    }

    // 3. 할당 크기 제한 (예: 최대 100 million)
    const int64 MaxElements = 100000000;
    if (NumElements > MaxElements)
    {
        UE_LOG(LogTemp, Error, TEXT("Output tensor size too large: %lld"), NumElements);
        return false;
    }

    // 4. 안전한 TArray 할당
    OutFeature.SetNumZeroed(static_cast<int32>(NumElements));

    UE_LOG(LogTemp, Warning, TEXT("Allocated OutFeature with %lld elements"), NumElements);

    TArray<FTensorBindingCPU> OutputBindings;
    OutputBindings.SetNum(1);

    OutputBindings[0].Data = OutFeature.GetData();
    OutputBindings[0].SizeInBytes = NumElements * sizeof(float);

    int32 RunStatus = ModelInstance->RunSync(InputBindings, OutputBindings);
    if (RunStatus != 0)
    {
        UE_LOG(LogTemp, Error, TEXT("RunSync failed with status: %d"), RunStatus);
        return false;
    }

    return true;
}

bool AProjectPlayerController::LoadPNG(
    const FString& ImagePath,
    TArray<float>& OutTensor)
{
    OutTensor.Empty();

    // 파일 로드
    TArray<uint8> FileData;
    if (!FFileHelper::LoadFileToArray(FileData, *ImagePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load file: %s"), *ImagePath);
        return false;
    }

    // PNG 디코딩
    IImageWrapperModule& ImageWrapperModule =
        FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

    TSharedPtr<IImageWrapper> ImageWrapper =
        ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (!ImageWrapper.IsValid() || !ImageWrapper->SetCompressed(FileData.GetData(), FileData.Num()))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to decode PNG"));
        return false;
    }

    TArray<uint8> RawData;

    if (!ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawData))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get raw image data"));
        return false;
    }

    int32 SrcWidth = ImageWrapper->GetWidth();
    int32 SrcHeight = ImageWrapper->GetHeight();

    UE_LOG(LogTemp, Warning, TEXT("Loaded Image: %dx%d"), SrcWidth, SrcHeight);

    // RGBA → FColor 배열
    TArray<FColor> SrcPixels;
    SrcPixels.SetNumUninitialized(SrcWidth * SrcHeight);
    FMemory::Memcpy(SrcPixels.GetData(), RawData.GetData(), RawData.Num());

    // 224x224로 리사이즈
    TArray<FColor> ResizedPixels;
    ResizedPixels.SetNumUninitialized(224 * 224);

    FImageUtils::ImageResize(
        SrcWidth,
        SrcHeight,
        SrcPixels,
        224,
        224,
        ResizedPixels,
        true
    );

    // Tensor 준비 (NCHW)
    OutTensor.SetNumZeroed(1 * 3 * 224 * 224);

    // MobileNet 정규화 (ImageNet 기준)
    const float Mean[3] = { 0.485f, 0.456f, 0.406f };
    const float Std[3] = { 0.229f, 0.224f, 0.225f };

    for (int32 y = 0; y < 224; ++y)
    {
        for (int32 x = 0; x < 224; ++x)
        {
            int32 PixelIndex = y * 224 + x;
            FColor Pixel = ResizedPixels[PixelIndex];

            float R = Pixel.R / 255.f;
            float G = Pixel.G / 255.f;
            float B = Pixel.B / 255.f;

            // 정규화
            R = (R - Mean[0]) / Std[0];
            G = (G - Mean[1]) / Std[1];
            B = (B - Mean[2]) / Std[2];

            // NCHW 배치 1
            OutTensor[0 * 224 * 224 + y * 224 + x] = R; // Channel 0
            OutTensor[1 * 224 * 224 + y * 224 + x] = G; // Channel 1
            OutTensor[2 * 224 * 224 + y * 224 + x] = B; // Channel 2
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Tensor Ready. Num: %d"), OutTensor.Num());
    return true;
}
