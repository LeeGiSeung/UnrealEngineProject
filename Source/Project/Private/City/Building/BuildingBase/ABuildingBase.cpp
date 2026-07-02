// Fill out your copyright notice in the Description page of Project Settings.


#include "City/Building/BuildingBase/ABuildingBase.h"
#include "Materials/MaterialInterface.h"
#include <string>

using namespace std;

AABuildingBase::AABuildingBase()
{

	PrimaryActorTick.bCanEverTick = true;

	BuildingID = -1;

    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
    RootComponent = BuildingMesh;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("BuildingBoxCollision"));
	Collision->SetBoxExtent(FVector(100.f, 100.f, 100.f));
	Collision->SetCollisionProfileName(TEXT("Trigger"));
	Collision->SetupAttachment(GetRootComponent());

}

void AABuildingBase::BeginPlay()
{
	Super::BeginPlay();
}

void AABuildingBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AABuildingBase::SetBuildingTransform(float _WidthX, float _WidthY, int floor, EBuildingType _BuildingType)
{
   
    if (floor < 1) floor = 1;

    if (!Collision) {
        UE_LOG(LogTemp, Error, TEXT("NO Collision"));
        return;
    }

    if (!BuildingMesh) {
        UE_LOG(LogTemp, Error, TEXT("NO BuildingMesh"));
        return;
    }

    iFloor = floor;
    WidthX = _WidthX;
    WidthY = _WidthY;
    BuildingType = _BuildingType;

    //UE_LOG(LogTemp, Warning, TEXT("BuildingType : %s"),
    //    *UEnum::GetValueAsString<EBuildingType>(BuildingType));

    

    Building();
}

void AABuildingBase::Building()
{
    if (iFloor < 1) iFloor = 1;
    if (!BuildingMesh || !Collision) {
        UE_LOG(LogTemp, Error, TEXT("Building() NO BuildingMesh NO Collision"));
        return;
    }

    for (UStaticMeshComponent* it : AdditionalLayers) {
        if (it) it->DestroyComponent();
    }

    AdditionalLayers.Empty();

    BuildingMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

    const float DefalutSize = 100.f;
    FVector CalculatedScale = FVector(WidthX / DefalutSize, WidthY / DefalutSize, fDefaultFloorHeight / 100.f);

    FVector MeshSize = BuildingMesh->GetStaticMesh()->GetBounds().GetBox().GetSize();

    //UE_LOG(LogTemp, Warning, TEXT("Mesh Size : %s"), *MeshSize.ToString());

    //UE_LOG(LogTemp, Warning, TEXT("WidthX = %f"), WidthX);
    //UE_LOG(LogTemp, Warning, TEXT("WidthY = %f"), WidthY);
    //UE_LOG(LogTemp, Warning, TEXT("CalculatedScale = %s"),
    //    *CalculatedScale.ToString());

    BuildingMesh->SetRelativeScale3D(CalculatedScale);

    const FBuildingSetting* Setting = BuildingSettingMap.Find(BuildingType);

    if (Setting && Setting->Material)
    {
        BuildingMesh->SetMaterial(0, Setting->Material);
    }

    for (int i = 0; i < iFloor; i++) {
        FString ComponentNameStr = FString::Printf(TEXT("BuildingMesh_%d"), i);
        FName ComponentName = FName(*ComponentNameStr);
        UStaticMeshComponent* NewFloor = NewObject<UStaticMeshComponent>(this, ComponentName);

        if (NewFloor) {
            // 부모에 장착 후 정식 등록
            NewFloor->SetupAttachment(GetRootComponent());
            NewFloor->RegisterComponent();

            NewFloor->SetStaticMesh(BuildingMesh->GetStaticMesh());
            //NewFloor->SetRelativeScale3D(FVector(0.4f, 0.4f, 1.f));
            //NewFloor->SetRelativeScale3D(BuildingMesh->GetRelativeScale3D());

            // Z축 위치 정렬
            NewFloor->SetRelativeLocation(FVector(0.0f, 0.0f, i * 100));
            NewFloor->SetMaterial(0, Setting->Material);

            AdditionalLayers.Add(NewFloor);
        }
    }
}

float AABuildingBase::GetWidthX()
{
    return WidthX;
}

float AABuildingBase::GetWidthY()
{
    return WidthY;
}

void AABuildingBase::SetWidgetX(float Value)
{
    WidthX = Value;
}

void AABuildingBase::SetWidgetY(float Value)
{
    WidthY = Value;
}
