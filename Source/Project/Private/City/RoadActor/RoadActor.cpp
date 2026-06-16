    // Fill out your copyright notice in the Description page of Project Settings.


    #include "City/RoadActor/RoadActor.h"
    #include "Components/SplineMeshComponent.h"
    #include "Components/SplineComponent.h"

    // Sets default values
    ARoadActor::ARoadActor()
    {
 	    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	    PrimaryActorTick.bCanEverTick = true;

	    RoadSpline = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
	    SetRootComponent(RoadSpline);

        //RoadSpline->SetMobility(EComponentMobility::Movable);
	
    }

    // Called when the game starts or when spawned
    void ARoadActor::BeginPlay()
    {
	    Super::BeginPlay();

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


