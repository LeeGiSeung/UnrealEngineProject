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

    void ARoadActor::SpawnRoadActor(TArray<FVector> Points, int32 RoadCount, double _RoadWidth)
    {
	    if (!RoadSpline || Points.Num() < 2) return;

        RoadWidth = _RoadWidth;
        //UE_LOG(LogTemp, Error, TEXT("%f"), _RoadWidth);
        RoadSpline->ClearSplinePoints(true);

        for (const FVector& Point : Points)
        {
            RoadSpline->AddSplinePoint(Point, ESplineCoordinateSpace::Local, false);
        }

        for (int32 i = 0; i < RoadSpline->GetNumberOfSplinePoints(); ++i)
        {
            RoadSpline->SetSplinePointType(i, ESplinePointType::Linear, false);
        }

 /*       UE_LOG(LogTemp, Log, TEXT("Calling BuildRoadMesh Event. Points: %d"), Points.Num());
        UE_LOG(LogTemp, Error, TEXT("%d, %d"), Points.Num(), RoadSpline->GetNumberOfSplineSegments());*/

        RoadSpline->UpdateSpline();

        BuildRoadMesh();

    }

    void ARoadActor::SetWorldPoints(TArray<FVector> value)
    {
	    WorldPoints = value;
    }

    void ARoadActor::OnConstruction(const FTransform& Transform)
    {
        Super::OnConstruction(Transform);
    }


