#include "City/MapWidget/MapViewer.h"
#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "ProjectCharacter.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"

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
	UE_LOG(LogTemp, Error, TEXT("%s"), *value.ToString());
}

void UMapViewer::ChangeMapImage()
{
	if (!MapImageArray.IsValidIndex(NowScollLevel)) return;

	UTexture2D* MarkerTexture = MapImageArray[NowScollLevel];

	if (!MarkerTexture) return;

	MapImage->SetBrushFromTexture(MarkerTexture);
}
