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

    //Building; BeginPlay() 에서 시작하는거임
}

void AABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABuildingBase::SetBuildingTransform(float _WidthX, float _WidthY, int floor)
{
    if (floor < 1) iFloor = 1;
    if (!BuildingMesh || !Collision) return;


    iFloor = floor;
    WidthX = _WidthX;
    WidthY = _WidthY;

    FVector NewExtent = FVector(_WidthX * 0.5f, _WidthY * 0.5f, (floor * 100) * 0.5f);
    Collision->SetBoxExtent(NewExtent);

    // 콜리전의 중심을 액터 루트(0,0,0)에 맞춥니다.
    Collision->SetRelativeLocation(FVector(0.f, 0.f, NewExtent.Z));

    FVector MeshRelativeLocation = FVector(-_WidthX * 0.5f, -_WidthY * 0.5f, 0.f);
    BuildingMesh->SetRelativeLocation(MeshRelativeLocation);

    FVector NewScale = FVector(_WidthX / 100.f, _WidthY / 100.f, (floor * 100) / 100.f);
    BuildingMesh->SetRelativeScale3D(NewScale);

    Building(); //이후 BeginPlay() 에서 옮겨옴
}

void AABuildingBase::Building()
{
    if (iFloor < 1) iFloor = 1;
    if (!BuildingMesh || !Collision) return;

    for (UStaticMeshComponent* it : AdditionalLayers) {
        if (it) it->DestroyComponent();
    }
    AdditionalLayers.Empty();

    BuildingMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

    const float DefalutSize = 100.f;
    FVector CalculatedScale = FVector(WidthX / DefalutSize, WidthY / DefalutSize, fDefaultFloorHeight / 100.f);

    BuildingMesh->SetRelativeScale3D(CalculatedScale);

    for (int i = 0; i < iFloor; i++) {
        FString ComponentNameStr = FString::Printf(TEXT("BuildingMesh_%d"), i);
        FName ComponentName = FName(*ComponentNameStr);
        UStaticMeshComponent* NewFloor = NewObject<UStaticMeshComponent>(this, ComponentName);

        if (NewFloor) {
            // 부모에 장착 후 정식 등록
            NewFloor->SetupAttachment(GetRootComponent());
            NewFloor->RegisterComponent();

            NewFloor->SetStaticMesh(BuildingMesh->GetStaticMesh());
            NewFloor->SetRelativeScale3D(FVector(0.4f, 0.4f, 1.f));

            // Z축 위치 정렬
            NewFloor->SetRelativeLocation(FVector(0.0f, 0.0f, i * 100));

            AdditionalLayers.Add(NewFloor);
        }
    }
}
