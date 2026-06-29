#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "City/CitySystemStruct/CitySystemStruct.h"

#include "MapViewer.generated.h"

class UImage;
class UPointMarker;
class UCanvasPanel;
class UUCityMapWidget;
class UUniformGridPanel;
class UPersonMarker;
class UUCityNewworkManager;
class AProjectCharacter;
class UTogetherManager;
class ATogetherRunBase;
class UCanvasPanelSlot;
class UTogetherActorMarker;
class UBossMarker;

UCLASS()
class PROJECT_API UMapViewer : public UUserWidget
{
	GENERATED_BODY()

//Function
public:
	virtual void NativeConstruct() override;

	void TogetherActorMarkerSetting();

	void SetFolderFileBaseMap();
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UTogetherManager* TogetherManager;

public:
	UFUNCTION(BlueprintCallable, Category = "MapViewFunction")
	void ScroolUp(FVector2D MousePostion);

	UFUNCTION(BlueprintCallable, Category = "MapViewFunction")
	void ScroolDown(FVector2D MousePostion);

	void ZoomViewSize();

	UFUNCTION(BlueprintCallable, Category = "MapViewFunction")
	void MapMove(FVector2D value);

	void UpdateTogetherActorUV();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void ChangeMapImage();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void MapRightButtonClick(const FVector2D& MousePosition);

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void MapLeftButtonClick();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void PositionToImagePath();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void ClearMapImage();

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void SpawnMapPointMarker(const FVector2D& _MousePosition);

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void DeleteMapPointMarker(UPointMarker* marker);

	UFUNCTION(BlueprintCallable)
	void UpMapMarkerMaxCount();

	UFUNCTION(BlueprintCallable)
	void DownMapMarkerMaxCount();

	UFUNCTION(BlueprintCallable)
	void SetOnPaintMarkerArray(TArray<FRoadNode> value);

	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void SetWBPMainHUD(UCityMapWidget* value);
	
	UFUNCTION(BlueprintCallable, Category = "MapViewer")
	void OnPaintNavigationCourse();

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	UFUNCTION()
	TArray<FVector2D> ComputeOnPaintLocationArray();

	UFUNCTION()
	void ChangeFilePath(FVector2D MousePostion, bool zoom);

	UFUNCTION()
	void ChangePointMarkerWorldPosition(FVector& value, FVector2D MapPosition);

	TArray<ATogetherRunBase*> *ChainArray;
public:
	//########################
	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	TSubclassOf<UPointMarker> PointMarkerClass;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	TArray<UTexture2D*> MapImageArray;

	UPROPERTY()
	TArray<UPointMarker*> PointMarkerArray;

	UPROPERTY(BlueprintReadWrite, Category = "OnPaint")
	TArray<FRoadNode> DFSNavigationLocationArray;

	UPROPERTY(BlueprintReadWrite, Category = "OnPaint")
	TArray<FVector2D> WidgetLocationArray;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	TArray<UImage*> MapGridPanelImage;
	//########################

	UPROPERTY()
	UUCityNewworkManager* CityNewworkManager;

	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel* MapImageGridPanel;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* MapViewerCanvasPanel;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 NowScrollLevel = 13; //스크롤 레벨

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 minScollLevel = 13;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 maxScollLevel = 17;

	UPROPERTY(BlueprintReadWrite, Category = "WheelDownClick")
	bool bMouseWheelDownClick = false;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly, Category = "MapViewer")
	UImage* MapImage;
	 
	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 iMapMarkerCount = 0;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	int32 iMapMarkerMaxCount = 1;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	UCityMapWidget* CityMapWidget;

	UFUNCTION()
	UTexture2D* LoadTextureFromFile(FString _FilePath);

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer")
	FVector2D CurrentMapRenderOffset;

	UPROPERTY(EditAnywhere)
	float TilePixelSize = 160.f;

	UPROPERTY(BlueprintReadWrite, Category = "MapViewer|PersonPNG")
	UTexture2D* PersonPNG;

	UPROPERTY(meta = (BindWidget))
	UPersonMarker* PersonMarker;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TogetherActorMarkerClass")
	TSubclassOf<UTogetherActorMarker> TogetherActorMarkerClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "TogetherActorMarkerClass")
	TSubclassOf<UBossMarker> BossMarkerClass;

	// 현재 맵 화면의 범위 (ChangeMapImage 때마다 갱신됨)
	float CurrentViewMinX, CurrentViewMaxX;
	float CurrentViewMinY, CurrentViewMaxY;

private:
	int32 MaxImageCountInFolder = 5;

	const double TileSize = 256.0;
	FVector CommitPlayerLocation;
	// 1. 요구사항에 맞춘 레벨 13 기준 원점 데이터 설정
	const int32 BaseZoom = 13;
	const int32 BaseStartFolder = 6977; // 시작 폴더 번호 변경
	const int32 BaseStartFile = 3174; // 시작 PNG 이름 변경

	const int32 BaseEndFolder = 6979; // 시작 폴더 번호 변경
	const int32 BaseEndFile = 3176; // 시작 PNG 이름 변경

	int32 TargetCenterFolder;
	int32 TargetCenterFile;

	TMap<int32, FFolderFileStartEndBase> FolderFileBaseMap;
	
	FTimerHandle MapMoveCoolTime;

	FTimerHandle UpdatePlayerPositionIcon;

	bool bMapMove = true;

	void bMapMoveTrue();
	


	void SpawnMarkerToMapViewer(UCanvasPanelSlot* CanvasSlot, FVector &Value);
	FVector InterpolateLocation(FVector Value);
	
	void UpdatePersonPosition(FVector PlayerLocation);
	void UpdatePersonRotation(FRotator PlayerRotation);

	void UpdateTogetherActorPosition(FVector TogetherLocation, ATogetherRunBase* Actor);
	void UpdateTogetherActorRotation(FRotator TogetherLocation, ATogetherRunBase* Actor);

	float WorldMinX = UE_MAX_FLT;
	float WorldMaxX = -UE_MAX_FLT;
	float WorldMinY = UE_MAX_FLT;
	float WorldMaxY = -UE_MAX_FLT;

	TMap<int32, FVector2D> ZoomCenterOffset;


	float UITimeImageSize = 160.f;

	void SetLocalMapSize(float _WorldMinX, float _WorldMaxX, float _WorldMinY, float _WorldMaxY);

	AProjectCharacter* Player;

	int32 FolderNumber;
	int32 FileNumber;

};
