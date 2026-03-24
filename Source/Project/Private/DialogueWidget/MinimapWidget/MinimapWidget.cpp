// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueWidget/MinimapWidget/MinimapWidget.h"
#include "Minimap/MinimapWorldSystem.h"
#include "MinimapType/MinimapType.h"

//Minimap
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "DialogueWidget/MinimapWidget/MinimapMarker.h"

void UMinimapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    UpdateMinimapMarkers();
}

void UMinimapWidget::NativeConstruct()
{
       
    Super::NativeConstruct();

    if (!PlayerMarker) return;
    UMinimapMarker* Player = CreateWidget<UMinimapMarker>(this, PlayerMarker);

    if (!Player) return;
    UCanvasPanelSlot* PlayerSlot =  Marker_Layer->AddChildToCanvas(Player);

    if (!PlayerSlot) return;

    PlayerSlot->SetAnchors(FAnchors(0.5f, 0.5f));
    PlayerSlot->SetAlignment(FVector2D(0.5F, 0.5F));
    PlayerSlot->SetPosition(FVector2D(0.f, 0.f));

}

void UMinimapWidget::UpdateMinimapMarkers()
{
    UMinimapWorldSystem* LocalMinimapSystem = GetWorld()->GetSubsystem<UMinimapWorldSystem>();
    TArray<FMinimapMakerData> LocalMarkerData = LocalMinimapSystem->GetActiveMarkers();
    FVector PlayerLocation = GetOwningPlayerPawn()->GetActorLocation();
    float PlayerYaw = GetOwningPlayerPawn()->GetActorRotation().Yaw;

    // 이번 프레임에 업데이트된 액터들을 기억하기 위한 셋
    TSet<AActor*> ActiveActorsThisFrame;

    for (const auto& Data : LocalMarkerData) {
        if (!Data.MarkerActor) continue;

        FVector MarkerActorLocation = Data.MarkerActor->GetActorLocation();
        float LocalDistance = FVector::Dist2D(PlayerLocation, MarkerActorLocation);

        // 거리 체크
        if (LocalDistance > MinimapDistance) continue;

        UMinimapMarker* CurrentMarker = nullptr;

        // 1. Map에서 이미 생성된 위젯이 있는지 확인
        if (MarkerMap.Contains(Data.MarkerActor)) {
            CurrentMarker = MarkerMap[Data.MarkerActor];
        }
        else {
            // 2. 없다면 새로 생성
            TSubclassOf<UMinimapMarker>* SelectMarker = MarkerConfig.Find(Data.MarkerType);
            if (SelectMarker && *SelectMarker) {
                CurrentMarker = CreateWidget<UMinimapMarker>(this, *SelectMarker);
                if (CurrentMarker) {
                    Marker_Layer->AddChildToCanvas(CurrentMarker);
                    MarkerMap.Add(Data.MarkerActor, CurrentMarker);
                }
            }
        }

        if (CurrentMarker) {
            // 위치 계산 로직 (수학 보정)
            FVector2D RelativePos = FVector2D(MarkerActorLocation.X - PlayerLocation.X, MarkerActorLocation.Y - PlayerLocation.Y);

            // 회전 적용: 위쪽을 정면으로 만들기 위해 -90도 보정 추가
            

            float CameraYaw = 0.f;
            if (APlayerController* PC = GetOwningPlayer()) {
                CameraYaw = PC->GetControlRotation().Yaw;
            }

            //FVector2D RotatedPos = RelativePos.GetRotated(-PlayerYaw - 90.f); ////PlayerActorRotation 기준
            FVector2D RotatedPos = RelativePos.GetRotated(-CameraYaw - 90.f);

            // 스케일링 (MinimapDistance 내의 거리를 MinimapRadius 픽셀 안으로 압축)
            float MinimapRadius = 130.f;
            float DistanceAlpha = LocalDistance / MinimapDistance; // 전체 거리 대비 비율
            FVector2D UIOffset = RotatedPos.GetSafeNormal() * (DistanceAlpha * MinimapRadius);

            UCanvasPanelSlot* MarkerSlot = Cast<UCanvasPanelSlot>(CurrentMarker->Slot);
            if (MarkerSlot) {
                MarkerSlot->SetPosition(UIOffset);
                MarkerSlot->SetAnchors(FAnchors(0.5f, 0.5f));
                MarkerSlot->SetAlignment(FVector2D(0.5f, 0.5f));
                // 얼라이먼트도 중앙으로 설정 (마커 아이콘 자체가 안 밀리게)
                MarkerSlot->SetAlignment(FVector2D(0.5f, 0.5f));
            }

            CurrentMarker->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

            // 이번 프레임에 활성화된 액터로 등록
            ActiveActorsThisFrame.Add(Data.MarkerActor);
        }
    }

    // 3. 이번 프레임에 업데이트되지 않은(범위 밖 혹은 제거된) 마커들 처리
    for (auto It = MarkerMap.CreateIterator(); It; ++It) {
        AActor* ActorKey = It.Key();
        UMinimapMarker* MarkerWidget = It.Value();

        if (!ActiveActorsThisFrame.Contains(ActorKey)) {
            // 액터 자체가 파괴되었다면 맵에서 제거하고 위젯 삭제
            if (!IsValid(ActorKey)) {
                if (MarkerWidget) MarkerWidget->RemoveFromParent();
                It.RemoveCurrent();
            }
            else {
                // 살아는 있는데 범위 밖이라면 숨기기만 함
                if (MarkerWidget) MarkerWidget->SetVisibility(ESlateVisibility::Collapsed);
            }
        }
    }
}
