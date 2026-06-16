// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/Boxcomponent.h"
#include "Components/StaticMeshcomponent.h"

#include "ABuildingBase.generated.h"

UCLASS()
class PROJECT_API AABuildingBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABuildingBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* Collision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* BuildingMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 BuildingID;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetBuildingTransform(float widthx, float widthy, int floor);

	void Building();

	UPROPERTY()
	TArray<UStaticMeshComponent*> AdditionalLayers;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Floor")
	int iFloor = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Floor")
	float fDefaultFloorHeight;

private:
	float fX;
	float fY;
	float WidthX;
	float WidthY;
	

};
