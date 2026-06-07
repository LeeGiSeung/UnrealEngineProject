#include "City/MapWidget/MapContent.h"
#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "ProjectCharacter.h"

//지도 데이터 : 현재 오픈된 구역(FOW, 안개 지역), 탐험 완료된 지역 정보.핀 / 
//마커 데이터 : 월드에 존재하는 퀘스트 위치, 상점 위치, 포탈(순간이동) 
//위치 정보의 배열.좌표 변환 로직 : 실제 3D 월드 좌표($X, Y$)를 지도용 2D 좌표($U, V$)로 변환하는 순수 수학 공식.
//게임 내 상호작용 : "플레이어가 포탈을 탔다", "새로운 지역을 발견했다" 같은 데이터 갱신 로직.

void UMapContent::NativeConstruct()
{
	Super::NativeConstruct();
}
