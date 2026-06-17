// Fill out your copyright notice in the Description page of Project Settings.


#include "City/RoadActor/RoadActor.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "City/Building/BuildingBase/ABuildingBase.h"
#include "City/UCityNewworkManager/UCityNewworkManager.h"

// Sets default values
ARoadActor::ARoadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RoadSpline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	SetRootComponent(RoadSpline);
	
}

// Called when the game starts or when spawned
void ARoadActor::BeginPlay()
{
	Super::BeginPlay();

    CityNewworkManager = Cast<UUCityNewworkManager>(GetWorld()->GetGameInstance()->GetSubsystemBase(UUCityNewworkManager::StaticClass()));

}

// Called every frame
void ARoadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoadActor::SpawnRoadActor(TArray<FVector> Points, int32 _RoadCount, double _RoadWidth)
{
	if (!RoadSpline || Points.Num() < 2) return;

    RoadCount = _RoadCount;
    RoadWidth = _RoadWidth;

    RoadSpline->ClearSplinePoints(true);

    for (const FVector& Point : Points)
    {
        RoadSpline->AddSplinePoint(Point, ESplineCoordinateSpace::Local, false);
    }

    for (int32 i = 0; i < RoadSpline->GetNumberOfSplinePoints(); ++i)
    {
        RoadSpline->SetSplinePointType(i, ESplinePointType::Linear, false);
    }

    RoadSpline->UpdateSpline();

    BP_SplineMesh();

    //겹친 빌딩있으면 해당 ABuildingBase Actor 삭제
    UWorld* World = GetWorld();
    if (!World) return;

    // 중복 제거 및 안전한 삭제를 위해 TSet 또는 TArray로 삭제 대상 수집
    TSet<AActor*> BuildingsToRemove;

    int32 NumSegments = RoadSpline->GetNumberOfSplinePoints() - 1;

    for (int32 i = 0; i < NumSegments; ++i)
    {
        // 1. 각 세그먼트의 시작점과 끝점 좌표 (월드 기준)
        FVector StartPos = RoadSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
        FVector EndPos = RoadSpline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);

        // 2. 박스의 중심점 및 배치 방향(Rotation) 계산
        FVector BoxCenter = (StartPos + EndPos) * 0.5f;
        FVector Direction = (EndPos - StartPos).GetSafeNormal();
        FRotator BoxRotation = Direction.Rotation();

        // 3. 높이(Z)는 최소화하여 평면 겹침 위주로 판단
        // Z Half-Extent를 100(총 높이 200) 정도로 낮춰, 도로 바닥면 평면 매칭에 집중합니다.
        float SegmentLength = FVector::Dist(StartPos, EndPos);
        FVector BoxHalfExtent = FVector(SegmentLength * 0.5f, _RoadWidth * 0.5f, 100.0f);

        // 4. Overlap Query 설정
        FCollisionShape CollisionBox = FCollisionShape::MakeBox(BoxHalfExtent);
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this); // 도로 자신은 제외

        TArray<FOverlapResult> OverlapResults;

        // ECC_WorldDynamic 또는 빌딩의 콜리전 채널에 맞춰 설정 (기본적으로 ECC_WorldDynamic/Static 추천)
        bool bHit = World->OverlapMultiByChannel(
            OverlapResults,
            BoxCenter,
            BoxRotation.Quaternion(),
            ECC_WorldDynamic,
            CollisionBox,
            QueryParams
        );

        if (bHit)
        {
            for (const FOverlapResult& Result : OverlapResults)
            {
                AActor* OverlappedActor = Result.GetActor();

                // 5. 검출된 액터가 ABuildingBase(혹은 그 자식 클래스)인지 확인
                if (OverlappedActor && OverlappedActor->IsA(AABuildingBase::StaticClass()))
                {
                    BuildingsToRemove.Add(OverlappedActor);
                }
            }
        }
    }

    for (AActor* Building : BuildingsToRemove)
    {
        if (AABuildingBase* BuildingBase = Cast<AABuildingBase>(Building))
        {
            // 맵이고 인덱스고 뭐고 주소 타고 들어가서 즉시 변경
            if (BuildingBase->MyRuntimeData.IsValid())
            {
                BuildingBase->MyRuntimeData->bOverlapRoad = true;
                BuildingBase->MyRuntimeData->SpawnedActor = nullptr;
            }

            BuildingBase->Destroy();
        }
    }
}

void ARoadActor::SetWorldPoints(TArray<FVector> value)
{
	WorldPoints = value;
}

void ARoadActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
}

void ARoadActor::ChangeRoadColor(int SegIdx ,FColor Color)
{

    // 안전 장치: 인덱스가 범위를 벗어나지 않았는지 확인
    if (!WorldPoints.IsValidIndex(SegIdx) || !WorldPoints.IsValidIndex(SegIdx + 1))
    {
        return;
    }

    // (참고) 만약 DebugBlockClass 액터 내부에 색상을 바꿀 수 있는 함수나 컴포넌트가 있다면 
    // 여기서 Color 인자를 넘겨주어 실제로 색을 바꿀 수도 있습니다.
    /*
    if (SpawnedDebugBlock)
    {
        // 예: SpawnedDebugBlock->SetMyColor(Color);
    }
    */
}


