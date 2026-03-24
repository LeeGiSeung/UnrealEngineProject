// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWidgetBlueprint.h"
#include "Blueprint/UserWidget.h"
#include "MinimapType/MinimapType.h"
#include "MinimapWidget.generated.h"

class UMinimapMarker;
class UCanvasPanel;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT_API UMinimapWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeConstruct() override;

public:
	//MinimapWidget
	UPROPERTY(BlueprintReadWrite, Category = "Minimap")
	TMap<EnumMinimapType, TSubclassOf<UMinimapMarker>> MarkerConfig;

	UPROPERTY(BlueprintReadWrite, Category = "Minimap")
	TSubclassOf<UMinimapMarker> PlayerMarker;

	// 위젯에 있는 Marker_Layer 캔버스 (이름 일치 필수!)
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* Marker_Layer;

	// 미리 생성해둔 마커 위젯들을 관리하는 배열
	UPROPERTY()
	TMap<AActor*, UMinimapMarker*> MarkerMap;

	// 현재 사용 중인 마커의 인덱스 추적
	int32 ActiveMarkerIdx = 0;

	void UpdateMinimapMarkers();

	UPROPERTY(BlueprintReadWrite)
	float MinimapDistance = 2000.f;

};
