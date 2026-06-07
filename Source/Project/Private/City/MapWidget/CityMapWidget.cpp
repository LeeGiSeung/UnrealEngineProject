#include "City/MapWidget/CityMapWidget.h"
#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "ProjectCharacter.h"

#include "City/MapWidget/MapContent.h"
#include "City/MapWidget/MapViewer.h"

//UI 토글 단축키 : 플레이어가 M 키를 누르면 MapViewer 위젯을 화면에 생성(Add to Viewport)하거나 숨기는 제어 로직.
//중재자(Bridge) : MapContent에 "나 지금 새로운 핀 꽂았어"라고 MapViewer가 신호를 보내면, 이를 전달받아 데이터를 갱신하고 다른 UI(예 : 퀘스트 추적창)에도 알려주는 역할.
//화면 레이아웃 관리 : 미니맵, 전체지도, 그리고 지도 옆에 붙는 '지역 탐색률 목록' 같은 서브 위젯들을 한데 묶어 관리.

void UCityMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CityNewworkManager = Cast<UUCityNewworkManager>(GetWorld()->GetGameInstance()->GetSubsystemBase(UUCityNewworkManager::StaticClass()));

}

void UCityMapWidget::NativeTick(const FGeometry& MyGeometry, float DelataTime)
{
	Super::NativeTick(MyGeometry, DelataTime);
}

void UCityMapWidget::NavigationFunction()
{

	if (CityNewworkManager) {
		AProjectCharacter* PlayerCharacter = Cast<AProjectCharacter>(GetOwningPlayerPawn());

		PlayerCharacter->FollowNavigation();
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("None CityNewworkManager"));
	}
}
