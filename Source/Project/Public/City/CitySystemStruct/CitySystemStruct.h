// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "CitySystemStruct.generated.h"

class AABuildingBase;

USTRUCT(BlueprintType)
struct FRoadData {
	GENERATED_BODY() // 최상단으로 이동

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	FString RoadName; // 명칭 (특수문자 제거됨)

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	double RoadWidth = 0.0; // 도로폭 (특수문자 제거됨)

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 RoadCount = 0; // 차로수 (특수문자 제거됨)

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TArray<FVector> Points; // 도로를 구성하는 정점들 (Line)
};

USTRUCT(BlueprintType)
struct FRoadNode {
	GENERATED_BODY() // 최상단으로 이동

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 NodeID = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	FVector Location = FVector::ZeroVector;

	// 이 노드에 연결된 무방향 에지들의 ID 목록
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TArray<int32> ConnectingEdgeIDs;
};

USTRUCT(BlueprintType)
struct FRoadEdge {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 EdgeID = 0;

	// Start/End 대신 무방향을 뜻하는 NodeA, NodeB 사용
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 NodeA = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 NodeB = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	float Distance = 0.0f;

	// 이 에지가 어떤 도로의 몇 번째 세그먼트인지 저장
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TObjectPtr<class ARoadActor> OwnerRoadActor = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 SegmentIndex = 0;
};

UENUM(BlueprintType)
enum class EMarkerType : uint8 {
	Toliet,
	Market,
	Bed,
	MapMarker,
	End
};

USTRUCT(BlueprintType)
struct FMapMarkerData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMarkerType MarkerType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MarkerID;          

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D GISCoordinate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* MarkerTexture;
};

USTRUCT(BlueprintType)
struct FRuntimeRoadData
{
	GENERATED_BODY()

	TArray<FVector> Points;
	int32 RoadCount = 1;
	float RoadWidth = 1.0f;
	FVector SpawnLocation = FVector::ZeroVector;

	// 현재 월드에 스폰된 도로 액터 포인터 (없으면 nullptr)
	UPROPERTY()
	class ARoadActor* SpawnedActor = nullptr;
};

USTRUCT(BlueprintType)
struct FFolderFileStartEndBase
{
	GENERATED_BODY()

	int32 BaseStartFolder = 0;
	int32 BaseStartFile = 0;

	int32 BaseEndFolder = 0;
	int32 BaseEndFile = 0;
};

USTRUCT(BlueprintType)
struct FMapCoordinate
{
	GENERATED_BODY()

	int32 Zoom;
	int32 Folder;
	int32 File;
	float U;
	float V;
};

UENUM(BlueprintType)
enum class EBuildingType : uint8
{
	None,
	OtherFacilities,
	FacilitiesRelatedtoAnimalsandPlants,
	AutomotiveRelatedFacilities,
	Accommodation,
	ReligiousFacilities,
	EducationalandResearchFacilities,
	HazardousMaterialsStorageandHandlingFacility,
	Facilitiesfortheelderlyandchildren,
	Transportationfacilities,
	SportsFacilities,
	WarehouseFacilities,
	RetailFacilities,
	OfficeFacilities,
	NightSoilandWasteTreatmentFacilities,
	Factory,
	House,
	MedicalFacilities,
	LivingFacility,
	TrainingFacility,
	CulturalandAssemblyFacilities,
	Funeralfacility,
	BroadcastingandCommunicationsFacilities,
	End
};

USTRUCT(BlueprintType)
struct FBuildingData {
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category = "Data")
	int32 FloorCount = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	FVector CenterLocation = FVector::ZeroVector;

	EBuildingType BuildingType = EBuildingType::None;

	UPROPERTY(BlueprintReadWrite, Category = "Data")
	TArray<FVector> Vertices; // 정렬된 최종 로컬 좌표들
};

USTRUCT(BlueprintType)
struct FBuildingSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AABuildingBase> BuildingClass;

	UPROPERTY(EditAnywhere)
	UMaterialInterface* Material;
};

USTRUCT(BlueprintType)
struct FRuntimeBuildingData
{
	GENERATED_BODY()

	FVector CenterLocation = FVector::ZeroVector;
	FVector SpawnLocation = FVector::ZeroVector;
	FRotator Rotation = FRotator::ZeroRotator;
	float WidthX = 0.f;
	float LengthY = 0.f;
	int32 FloorCount = 1;

	bool bOverlapRoad = false;

	EBuildingType BuildingType = EBuildingType::None;

	// 현재 월드에 스폰된 액터의 포인터 (스폰되지 않았다면 nullptr)
	UPROPERTY()
	class AABuildingBase* SpawnedActor = nullptr;
};