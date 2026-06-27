// Fill out your copyright notice in the Description page of Project Settings.


#include "City/MapWidget/Marker/MapViewer/TogetherActor/TogetherActorMarker.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

void UTogetherActorMarker::NativeConstruct()
{
	Super::NativeConstruct();



}

void UTogetherActorMarker::TurnTogetherImage(FRotator Value)
{
    if (!TogetherActorImage) return;

    const float Radius = 30.f;

    const float Rad = FMath::DegreesToRadians(Value.Yaw);

    //FVector2D Offset;
    //Offset.X = FMath::Cos(Rad) * Radius;
    //Offset.Y = FMath::Sin(Rad) * Radius;

    //if (UCanvasPanelSlot* TogetherActorImagewSlot = Cast<UCanvasPanelSlot>(TogetherActorImage->Slot))
    //{
    //    TogetherActorImagewSlot->SetPosition(Offset);
    //}

    FWidgetTransform Transform = TogetherActorImage->RenderTransform;
    Transform.Angle = Value.Yaw;
    TogetherActorImage->SetRenderTransform(Transform);
}
