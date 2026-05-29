// Fill out your copyright notice in the Description page of Project Settings.


#include "City/Building/BuildingBase/ABuildingBase.h"

#include <string>

using namespace std;

AABuildingBase::AABuildingBase()
{

	PrimaryActorTick.bCanEverTick = true;

	BuildingID = -1;

    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
    BuildingMesh->SetupAttachment(GetRootComponent());

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BuildingBoxCollision"));
	Collision->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	Collision->SetCollisionProfileName(TEXT("Trigger"));
	Collision->SetupAttachment(GetRootComponent());

}

void AABuildingBase::BeginPlay()
{
	Super::BeginPlay();

    //SetActorScale3D(FVector(1.0f, 1.0f, 1.0f));
    SetBuildingTransform(200,200,5);
}

void AABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABuildingBase::SetBuildingTransform(float WidthX, float WidthY, int floor)
{
    if (floor < 1) floor = 1;
    if (!BuildingMesh || !Collision) return;

    for (UStaticMeshComponent* it : AdditionalLayers) {
        if (it) it->DestroyComponent(); 
    }
    AdditionalLayers.Empty();

    BuildingMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

    const float DefalutSize = 100.f;
    FVector CalculatedScale = FVector(WidthX / DefalutSize, WidthY / DefalutSize, 1.f);

    BuildingMesh->SetRelativeScale3D(CalculatedScale);

    for (int i = 0; i < floor; i++) {
        FString ComponentNameStr = FString::Printf(TEXT("BuildingMesh_%d"), i);
        FName ComponentName = FName(*ComponentNameStr);
        UStaticMeshComponent* NewFloor = NewObject<UStaticMeshComponent>(this, ComponentName);

        if (NewFloor) {
            // 부모에 장착 후 정식 등록
            NewFloor->SetupAttachment(GetRootComponent());
            NewFloor->RegisterComponent();

            NewFloor->SetStaticMesh(BuildingMesh->GetStaticMesh());
            NewFloor->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

            // Z축 위치 정렬
            NewFloor->SetRelativeLocation(FVector(0.0f, 0.0f, i * 100.0f));

            AdditionalLayers.Add(NewFloor);

            // [수정] 정상적으로 생성되고 등록 완료된 메쉬만 안전하게 배열에 추가합니다!
            AdditionalLayers.Add(NewFloor);
        }
    }
}
