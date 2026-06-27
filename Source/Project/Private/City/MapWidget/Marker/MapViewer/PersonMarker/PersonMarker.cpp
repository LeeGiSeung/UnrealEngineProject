// Fill out your copyright notice in the Description page of Project Settings.


#include "City/MapWidget/Marker/MapViewer/PersonMarker/PersonMarker.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UPersonMarker::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPersonMarker::TurnArrowImage(FRotator Value)
{

    if (!ArrowImage) return;

    const float Radius = 30.f; // PersonMarker와 화살표 사이 거리

    const float Rad = FMath::DegreesToRadians(Value.Yaw);

    FVector2D Offset;
    Offset.X = FMath::Cos(Rad) * Radius;
    Offset.Y = FMath::Sin(Rad) * Radius;

    if (UCanvasPanelSlot* ArrowSlot = Cast<UCanvasPanelSlot>(ArrowImage->Slot))
    {
        ArrowSlot->SetPosition(Offset);
    }

    FWidgetTransform Transform = ArrowImage->RenderTransform;
    Transform.Angle = Value.Yaw;
    ArrowImage->SetRenderTransform(Transform);

}
