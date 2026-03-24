// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MinimapMarker.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UMinimapMarker : public UUserWidget
{
	GENERATED_BODY()
	
public:
    // 블루프린트에서 호출하거나 구현할 이벤트
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void UpdateMarkerAppearance(EnumMinimapType Type);

    // 나중에 보스 스킬 연동 등을 위해 상태를 업데이트하는 함수
    UFUNCTION(BlueprintCallable, Category = "Minimap")
    void SetIsAlert(bool bIsAlert);

};
