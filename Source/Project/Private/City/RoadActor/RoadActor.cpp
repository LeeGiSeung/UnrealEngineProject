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
		// 액터의 중심(SpawnLocation) 기준으로 로컬 좌표로 변환하여 추가하거나, 
		// Spawn 시점에 로케이션을 (0,0,0)으로 잡았다면 그대로 넣어도 됩니다.
		// 여기서는 로컬 좌표 기준으로 추가합니다.
		RoadSpline->AddSplinePoint(Point, ESplineCoordinateSpace::Local, false);
	}
	
	// 포인트 타입을 직선(Linear)으로 변경하여 GeoJSON 특유의 꺾인 선을 살립니다.
	// 곡선도로를 원하시면 ESplinePointType::Curve로 유지해도 됩니다.
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

	// 3. 각 세그먼트(구간)를 순회하며 Spline Mesh Component 생성 및 배치
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

		// 4. 도로 너비(RoadWidth) 조절
		// 사용하는 RoadMesh의 기본 가로폭이 100단위(1미터)라고 가정할 때의 스케일 계산입니다.
		// 예: GeoJSON의 RoadWidth가 2.0(미터)라면, 에셋의 크기에 맞춰 단위를 보정해 줍니다.
		// 언리얼 기본 단위는 cm이므로 2m = 200cm입니다.
		float WidthScale = (RoadWidth * 100.0f) / 100.0f; // 에셋 기본 가로 너비 크기에 맞춰 제수(분모)를 조정하세요.
		
		FVector2D Scale(WidthScale, 1.0f); // X축 너비를 스케일링 (Z축은 두께이므로 1.0)
		SplineMeshComp->SetStartScale(Scale);
		SplineMeshComp->SetEndScale(Scale);

		// 충돌(Collision) 설정이 필요하다면 추가
		SplineMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		SplineMeshComp->UpdateMesh();
	}
}

