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
	//폴더 이름(7030, 7031...) = 바둑판의 가로(X축) 열 번호
	//이미지 이름(3236.png, 3237.png...) = 바둑판의 세로(Y축) 행 번호

	//레벨이 1씩 오를수록 이전 레벨의 폴더이름 ,이미지이름이 *2 씩 오름

	//12945

    const double TileSize = 256.0;

    // 우리가 확실히 검증한 레벨 13의 원점 데이터
    const int32 BaseZoom = 13;
    const int32 BaseStartX = 7030;
    const int32 BaseStartY = 3236;

    // 줌 레벨 차이 계산 (예: 현재 14레벨이면 14 - 13 = 1)
    int32 ZoomDiff = NowScollLevel - BaseZoom;

    // 2의 거듭제곱 배수 구하기 (1단계 차이나면 2배, 2단계 차이나면 4배)
    int32 ScaleMultiplier = FMath::RoundToInt(FMath::Pow(2.0, ZoomDiff));

    // 현재 줌 레벨에 맞는 진짜 시작 타일 번호 자동 계산
    // 현재 레벨이 14라면 자동으로 7030*2 = 14060 / 3236*2 = 6472 가 됩니다!
    int32 CurrentStartX = BaseStartX * ScaleMultiplier;
    int32 CurrentStartY = BaseStartY * ScaleMultiplier;

    // 마우스가 클릭한 내부 오프셋 칸수 계산 (0, 1, 2...)
    int32 TileOffsetX = FMath::FloorToInt(_MousePosition.X / TileSize);
    int32 TileOffsetY = FMath::FloorToInt(_MousePosition.Y / TileSize);

    // 최종 하드디스크에 매핑될 가로줄(X), 세로줄(Y) 인덱스
    int32 FinalTileX = CurrentStartX + TileOffsetX;
    int32 FinalTileY = CurrentStartY + TileOffsetY;

    // 결과물 디버그 로그
    UE_LOG(LogTemp, Error, TEXT("[MapViewer] Zoom: %d | X(Folder): %d | Y(FileName): %d.png"),
        NowScollLevel, FinalTileX, FinalTileY);

    // 경로 조립
    FString TargetPath = FPaths::ProjectContentDir() + FString::Printf(TEXT("Movies/%d/%d/%d.png"),
        NowScollLevel, FinalTileX, FinalTileY);

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

void UMapViewer::TestiMapMarkerMaxCount()
{
    UE_LOG(LogTemp, Error, TEXT("%d"), iMapMarkerMaxCount);
}




