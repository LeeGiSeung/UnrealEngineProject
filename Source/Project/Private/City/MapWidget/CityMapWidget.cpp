#include "City/MapWidget/CityMapWidget.h"
#include "City/UCityNewworkManager/UCityNewworkManager.h"
#include "ProjectCharacter.h"

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
