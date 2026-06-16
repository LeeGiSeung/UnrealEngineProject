#include "City/MapWidget/MapViewer.h"
#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "ProjectCharacter.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "City/MapWidget/Marker/MapViewer/PointMarker.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include <cmath>
#include "Components/CanvasPanel.h"
#include "ProjectPlayerController.h"
#include "City/MapWidget/CityMapWidget.h"

//화면 렌더링 : 지도 이미지(Texture)를 보여주고, 줌인 / 줌아웃(Zoom), 드래그로 지도 이동(Pan)하는 기능.
//마커 표시 : MapContent에서 마커 데이터를 받아와 화면에 아이콘 위젯들을 생성하고 배치하는 기능.
//UI 입력 처리 : 지도 스크롤, 마우스 우클릭으로 핀 꽂기, 마커 클릭 시 해당 장소 툴팁 띄우기.

void UMapViewer::NativeConstruct()
{
	Super::NativeConstruct();

	ChangeMapImage();

}

void UMapViewer::ScroolUp()
{
	if (NowScollLevel - 1 < minScollLevel) return;
	NowScollLevel--;

	ChangeMapImage();
}

void UMapViewer::ScroolDown()
{
	if (NowScollLevel + 1 > maxScollLevel) return;
	NowScollLevel++;

	ChangeMapImage();
}

void UMapViewer::MapMove(FVector2D value)
{
	
}

void UMapViewer::ChangeMapImage()
{
	if (!MapImageArray.IsValidIndex(NowScollLevel)) return;

	UTexture2D* MarkerTexture = MapImageArray[NowScollLevel];

	if (!MarkerTexture) return;

	MapImage->SetBrushFromTexture(MarkerTexture);
}

void UMapViewer::MapRightButtonClick(const FVector2D& _MousePosition)
{
	MousePositionToImagePath(_MousePosition);

    SpawnMapPointMarker(_MousePosition);
}

void UMapViewer::MapLeftButtonClick()
{

}

void UMapViewer::MousePositionToImagePath(const FVector2D& _MousePosition)
{
    const double TileSize = 256.0;

    // 1. 요구사항에 맞춘 레벨 13 기준 원점 데이터 설정
    const int32 BaseZoom = 13;
    const int32 BaseStartX = 6922; // 시작 폴더 번호 변경
    const int32 BaseStartY = 3130; // 시작 PNG 이름 변경

    // 2. NowScollLevel 변수를 기반으로 현재 레벨 판단 및 13~17 레벨 범위 제한 (안전장치)
    int32 ClampedScrollLevel = FMath::Clamp(NowScollLevel, 13, 17);

    // 줌 레벨 차이 계산
    int32 ZoomDiff = ClampedScrollLevel - BaseZoom;

    // 2의 거듭제곱 배수 구하기 (레벨이 1 오를 때마다 2배씩 증가)
    int32 ScaleMultiplier = FMath::RoundToInt(FMath::Pow(2.0, ZoomDiff));

    // 현재 줌 레벨에 맞는 진짜 시작 타일 번호 자동 계산
    int32 CurrentStartX = BaseStartX * ScaleMultiplier;
    int32 CurrentStartY = BaseStartY * ScaleMultiplier;

    // 마우스가 클릭한 내부 오프셋 칸수 계산 (0, 1, 2...)
    int32 TileOffsetX = FMath::FloorToInt(_MousePosition.X / TileSize);
    int32 TileOffsetY = FMath::FloorToInt(_MousePosition.Y / TileSize);

    // 최종 매핑될 가로줄(X, 폴더명), 세로줄(Y, 파일명) 인덱스
    int32 FinalTileX = CurrentStartX + TileOffsetX;
    int32 FinalTileY = CurrentStartY + TileOffsetY;

    // 결과물 디버그 로그
    UE_LOG(LogTemp, Error, TEXT("[MapViewer] Zoom: %d | X(Folder): %d | Y(FileName): %d.png"),
        ClampedScrollLevel, FinalTileX, FinalTileY);

    FString WindowsUserDocDir = FPlatformProcess::UserDir();
    FString TargetPath = FPaths::Combine(*WindowsUserDocDir, TEXT("KoreaLoadPng"),
        *FString::Printf(TEXT("%d/%d/%d.png"), ClampedScrollLevel, FinalTileX, FinalTileY));


}

void UMapViewer::SpawnMapPointMarker(const FVector2D& _MousePosition)
{
    if (!PointMarkerClass) {
        UE_LOG(LogTemp, Error, TEXT("PointMarkerClass Is None"));
        return;
    }

    int PointMarkerArrayNum = PointMarkerArray.Num();

    if (PointMarkerArrayNum >= iMapMarkerMaxCount) { //이미 Marker가 iMapMarkerMaxCount 이상 설치돼있으면 //젤 처음 marker를 지움
        DeleteMapPointMarker(PointMarkerArray[0]);

        PointMarkerArrayNum = PointMarkerArray.Num();
    }

    UPointMarker* PointMarkerWidget = CreateWidget<UPointMarker>(GetWorld(), PointMarkerClass);
    UPanelSlot* RawChild = MapViewerCanvasPanel->AddChild(PointMarkerWidget);

    UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(RawChild);

    if (CanvasSlot) {

        FVector2D ScreenMousePosition;
        AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetOwningPlayer());
        PC->GetMousePosition(ScreenMousePosition.X, ScreenMousePosition.Y);

        FVector2D LocalScreenMousePostion = MapViewerCanvasPanel->GetCachedGeometry().AbsoluteToLocal(ScreenMousePosition);

        CanvasSlot->SetPosition(LocalScreenMousePostion);
        CanvasSlot->SetAlignment(FVector2D(0.5, 1.0));
        
        PointMarkerWidget->SetSpawnMarker(iMapMarkerCount, this);
        
        PointMarkerArray.Add(PointMarkerWidget);
    }
}

void UMapViewer::DeleteMapPointMarker(UPointMarker* marker)
{
    if (!marker) return;
    PointMarkerArray.Remove(marker);

    marker->RemoveFromParent();
}

void UMapViewer::UpMapMarkerMaxCount()
{
    if (iMapMarkerMaxCount + 1 > 4) return;
    iMapMarkerMaxCount += 1;

}

void UMapViewer::DownMapMarkerMaxCount()
{
    if (iMapMarkerMaxCount - 1 < 2) return;
    iMapMarkerMaxCount -= 1;

}

void UMapViewer::SetOnPaintMarkerArray(TArray<FRoadNode> value)
{
    if (value.Num() == 0) {
        UE_LOG(LogTemp, Error, TEXT("TArray<FRoadNode> IS NoNE"));
        return;
    }

    DFSNavigationLocationArray.Empty();

    DFSNavigationLocationArray = value;

    OnPaintNavigationCourse();
}

void UMapViewer::SetWBPMainHUD(UCityMapWidget* value)
{
    if (!value) return;

    CityMapWidget = value;
}

void UMapViewer::OnPaintNavigationCourse()
{
    UE_LOG(LogTemp, Error, TEXT("OnPaintNavigationCourse In %d"), DFSNavigationLocationArray.Num());

    WidgetLocationArray.Empty();
    WidgetLocationArray = ComputeOnPaintLocationArray(); //구현해야함
}

int32 UMapViewer::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    int32 ResultLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

    if (WidgetLocationArray.Num() >= 2)
    {
        FSlateDrawElement::MakeLines(
            OutDrawElements,
            ResultLayerId,
            AllottedGeometry.ToPaintGeometry(),
            WidgetLocationArray, // <--- 반복문 없이 여기서 한 번에 처리
            ESlateDrawEffect::None,
            FLinearColor::Yellow,
            true,
            3.0f
        );
    }

    return int32();
}

TArray<FVector2D> UMapViewer::ComputeOnPaintLocationArray()
{
    TArray<FVector2D> returnArray;

    return returnArray;
}




