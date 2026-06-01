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

void ARoadActor::SpawnRoadActor(TArray<FVector> Points, double RoadWidth)
{
	if (!RoadSpline || Points.Num() < 2) return;

	// 1. 기존 스플라인 포인트 리셋 후 GeoJSON 데이터로 채우기
	RoadSpline->ClearSplinePoints(true);
	
	for (const FVector& Point : Points)
	{
		RoadSpline->AddSplinePoint(Point, ESplineCoordinateSpace::Local, false);
	}
	
	for (int32 i = 0; i < RoadSpline->GetNumberOfSplinePoints(); ++i)
	{
		RoadSpline->SetSplinePointType(i, ESplinePointType::Linear, false);
	}
	
	// 변경사항 반영
	RoadSpline->UpdateSpline();

	// 2. 메시 에셋이 없으면 스플라인 메시를 생성하지 않음 (방어 코드)
	if (!RoadMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO RoadMesh"));
		return;
	}

	int32 NumSegments = RoadSpline->GetNumberOfSplinePoints() - 1;

	for (int32 i = 0; i < NumSegments; ++i)
	{
		// 동적으로 Spline Mesh Component 생성
		USplineMeshComponent* SplineMeshComp = NewObject<USplineMeshComponent>(this);
		if (!SplineMeshComp) continue;

		// 모빌리티 설정 및 메시 할당
		SplineMeshComp->SetMobility(EComponentMobility::Movable);
		SplineMeshComp->SetStaticMesh(RoadMesh);
		
		// 계층 구조에 등록 및 액터에 부착
		SplineMeshComp->RegisterComponent();
		SplineMeshComp->AttachToComponent(RoadSpline, FAttachmentTransformRules::KeepRelativeTransform);

		// 현재 구간의 시작점과 끝점의 위치, 접선 정보 가져오기
		FVector StartPos, StartTangent, EndPos, EndTangent;
		RoadSpline->GetLocationAndTangentAtSplinePoint(i, StartPos, StartTangent, ESplineCoordinateSpace::Local);
		RoadSpline->GetLocationAndTangentAtSplinePoint(i + 1, EndPos, EndTangent, ESplineCoordinateSpace::Local);

		// 스플라인 메시 컴포넌트에 시작/끝 데이터 주입
		SplineMeshComp->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);

		float WidthScale = (RoadWidth * 100.0f) / 100.0f;
		
		FVector2D Scale(WidthScale, 1.0f);
		SplineMeshComp->SetStartScale(Scale);
		SplineMeshComp->SetEndScale(Scale);

		SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		SplineMeshComp->UpdateMesh();
	}
}

void ARoadActor::SetWorldPoints(TArray<FVector> value)
{
	WorldPoints = value;
}

