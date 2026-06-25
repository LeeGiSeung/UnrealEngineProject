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
#include "HAL/FileManager.h" // 파일 개수 검사를 위해 필요
#include "City/MapWidget/Marker/MapViewer/PersonMarker/PersonMarker.h"
#include "ProjectCharacter.h"

//Image
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"
#include "Components/UniformGridPanel.h"

//화면 렌더링 : 지도 이미지(Texture)를 보여주고, 줌인 / 줌아웃(Zoom), 드래그로 지도 이동(Pan)하는 기능.
//마커 표시 : MapContent에서 마커 데이터를 받아와 화면에 아이콘 위젯들을 생성하고 배치하는 기능.
//UI 입력 처리 : 지도 스크롤, 마우스 우클릭으로 핀 꽂기, 마커 클릭 시 해당 장소 툴팁 띄우기.

void UMapViewer::NativeConstruct()
{
	Super::NativeConstruct();

    SetFolderFileBaseMap();

    ChangeMapImage();
    
    Player = Cast<AProjectCharacter>(GetOwningPlayerPawn());

    CityNewworkManager = Cast<UUCityNewworkManager>(GetWorld()->GetGameInstance()->GetSubsystemBase(UUCityNewworkManager::StaticClass()));

    if (Player) {
        Player->OnPlayerMoved.AddUObject(this, &UMapViewer::UpdatePersonPosition);
    }

    if (CityNewworkManager) {

        CityNewworkManager->GetMapBounds(WorldMinX, WorldMaxX, WorldMinY, WorldMaxY);
        SetLocalMapSize(WorldMinX, WorldMaxX, WorldMinY, WorldMaxY);

        CityNewworkManager->OnLocalXYSetting.AddUObject(this, &UMapViewer::SetLocalMapSize);
    }
    

}

void UMapViewer::SetFolderFileBaseMap()
{
    FFolderFileStartEndBase StartEndBase;
    StartEndBase.BaseStartFolder = BaseStartFolder;
    StartEndBase.BaseStartFile = BaseStartFile;
    StartEndBase.BaseEndFolder = BaseEndFolder;
    StartEndBase.BaseEndFile = BaseEndFile;
    FolderFileBaseMap.Add(minScollLevel, StartEndBase);

    TargetCenterFolder = 6978;
    TargetCenterFile = 3175;

    for (int ScroolLevel = minScollLevel + 1; ScroolLevel <= maxScollLevel; ScroolLevel++) {
        int sqrt = ScroolLevel - minScollLevel;
        int sq = pow(2, sqrt);
        FFolderFileStartEndBase StartEndFor;
        StartEndFor.BaseStartFolder = BaseStartFolder * sq;
        StartEndFor.BaseStartFile = BaseStartFile * sq;
        StartEndFor.BaseEndFolder = BaseEndFolder * sq;
        StartEndFor.BaseEndFile = BaseEndFile * sq;

        FolderFileBaseMap.Add(ScroolLevel, StartEndFor);

    }
}

void UMapViewer::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMapViewer::ChangeFilePath(FVector2D MousePosition, bool bZoomIn)
{
    // 그리드 패널의 현재 화면 상 절대 좌표를 가져옵니다.
    //FVector2D PanelScreenPos = MapImageGridPanel->GetCachedGeometry().GetAbsolutePosition();
    FVector2D PanelScreenPos;
    PanelScreenPos.X = 20.f;
    PanelScreenPos.Y = 10.f;

    // 마우스가 5x5 그리드 중 몇 번째 칸(0 ~ 4)에 있는지 인덱스 계산
    int32 MouseGridX = FMath::FloorToInt((MousePosition.X - PanelScreenPos.X) / TilePixelSize);
    int32 MouseGridY = FMath::FloorToInt((MousePosition.Y - PanelScreenPos.Y) / TilePixelSize);

    // 마우스가 패널 영역을 벗어났을 때를 대비한 안전장치
    MouseGridX = FMath::Clamp(MouseGridX, 0, 4);
    MouseGridY = FMath::Clamp(MouseGridY, 0, 4);

    // 현재 마우스 밑에 있던 실제 타일(파일) 번호 역산
    const int32 CenterIndex =
        MaxImageCountInFolder / 2;

    int32 CurrentMouseFolder =
        TargetCenterFolder + (MouseGridY - CenterIndex);

    int32 CurrentMouseFile =
        TargetCenterFile + (MouseGridX - CenterIndex);

    UE_LOG(LogTemp, Error, TEXT("Before TargetCenterFolder %d: , TargetCenterFile : %d"), TargetCenterFolder, TargetCenterFile);

    if (bZoomIn)
    {
        TargetCenterFolder = CurrentMouseFolder * 2;
        TargetCenterFile = CurrentMouseFile * 2;
        
        UE_LOG(LogTemp, Error, TEXT("After Zoomin TargetCenterFolder %d: , TargetCenterFile : %d"), TargetCenterFolder, TargetCenterFile);

    }
    else
    {
        TargetCenterFolder = CurrentMouseFolder / 2;
        TargetCenterFile = CurrentMouseFile / 2;

        UE_LOG(LogTemp, Error, TEXT("After Zoomout TargetCenterFolder %d: , TargetCenterFile : %d"), TargetCenterFolder, TargetCenterFile);
    }

    ChangeMapImage();
}

void UMapViewer::ChangePointMarkerWorldPosition(
    FVector& OutLocation,
    FVector2D MapPosition)
{
    const int32 CenterIndex =
        MaxImageCountInFolder / 2;

    //----------------------------------
    // 현재 타일 찾기
    //----------------------------------

    int32 TileX =
        FMath::FloorToInt(MapPosition.X / TilePixelSize);

    int32 TileY =
        FMath::FloorToInt(MapPosition.Y / TilePixelSize);

    TileX = FMath::Clamp(TileX, 0, MaxImageCountInFolder - 1);
    TileY = FMath::Clamp(TileY, 0, MaxImageCountInFolder - 1);

    //----------------------------------
    // 실제 Folder / File
    //----------------------------------

    int32 Folder =
        TargetCenterFolder + (TileX - CenterIndex);

    int32 File =
        TargetCenterFile + (TileY - CenterIndex);

    //----------------------------------
    // 타일 내부 UV
    //----------------------------------

    float LocalU =
        FMath::Fmod(MapPosition.X, TilePixelSize)
        / TilePixelSize;

    float LocalV =
        FMath::Fmod(MapPosition.Y, TilePixelSize)
        / TilePixelSize;

    FFolderFileStartEndBase* Range =
        FolderFileBaseMap.Find(NowScrollLevel);

    if (!Range)
    {
        return;
    }

    double GlobalU =
        (
            (Folder - Range->BaseStartFolder)
            + LocalU
            )
        /
        (
            double)(Range->BaseEndFolder - Range->BaseStartFolder);

    double GlobalV =
        (
            (File - Range->BaseStartFile)
            + LocalV
            )
        /
        (
            double)(Range->BaseEndFile - Range->BaseStartFile);

    GlobalU = FMath::Clamp(GlobalU, 0.0, 1.0);
    GlobalV = FMath::Clamp(GlobalV, 0.0, 1.0);

    OutLocation.X = FMath::Lerp(WorldMinX, WorldMaxX, GlobalU);
        

    OutLocation.Y = FMath::Lerp(WorldMinY, WorldMaxY, GlobalV);
        
    OutLocation.X += ZoomCenterOffset[NowScrollLevel].X;
    OutLocation.Y += ZoomCenterOffset[NowScrollLevel].Y;

    const FVector Offset(
        17769.538613,
        -87832.927392,
        0.f
    );


    OutLocation -= Offset;
}

void UMapViewer::ScroolUp(FVector2D MousePostion)
{
    if (NowScrollLevel + 1 > maxScollLevel) return;
    NowScrollLevel++;

    ChangeFilePath(MousePostion, true);
    ChangeMapImage();

    UpdatePersonPosition(CommitPlayerLocation);
}

void UMapViewer::ScroolDown(FVector2D MousePostion)
{
    if (NowScrollLevel - 1 < minScollLevel) return;
    NowScrollLevel--;

    ChangeFilePath(MousePostion, false);
    ChangeMapImage();

    UpdatePersonPosition(CommitPlayerLocation);
}

void UMapViewer::MapMove(FVector2D value)
{
    if (value.X == 0 && value.Y == 0) return;

    if (bMapMove == false) return;
    bMapMove = false;

    GetWorld()->GetTimerManager().SetTimer(MapMoveCoolTime, this, &UMapViewer::bMapMoveTrue, 0.15f, false);

    UE_LOG(LogTemp, Error, TEXT("%f, %f"), value.X, value.Y);

    // 상하좌우
    if (value.X == 0 && value.Y < 0) {
        TargetCenterFile += 1; // 상
    }
    else if (value.X == 0 && value.Y > 0) {
        TargetCenterFile -= 1; // 하
    }
    else if (value.X < 0 && value.Y == 0) {
        TargetCenterFolder += 1; // 좌`
    }
    else if (value.X > 0 && value.Y == 0) {
        TargetCenterFolder -= 1; // 우
    }
    // 대각선
    else if (value.X < 0 && value.Y < 0) {
        TargetCenterFolder += 1; // 좌상
        TargetCenterFile += 1;
    }
    else if (value.X > 0 && value.Y < 0) {
        TargetCenterFolder -= 1; // 우상
        TargetCenterFile += 1;
    }
    else if (value.X > 0 && value.Y > 0) {
        TargetCenterFolder -= 1; // 우하
        TargetCenterFile -= 1;
    }
    else if (value.X < 0 && value.Y > 0) {
        TargetCenterFolder += 1; // 좌하
        TargetCenterFile -= 1;
    }
    ChangeMapImage();
}

void UMapViewer::ChangeMapImage()
{
    ClearMapImage();

    int32 ClampedScrollLevel =
        FMath::Clamp(NowScrollLevel, 13, 17);

    const int32 CenterIndex =
        MaxImageCountInFolder / 2;

    FString ProjDir = FPaths::ProjectDir();

    for (int32 Y = 0; Y < MaxImageCountInFolder; Y++)
    {
        for (int32 X = 0; X < MaxImageCountInFolder; X++)
        {
            int32 GridIndex =
                Y * MaxImageCountInFolder + X;

            if (!MapGridPanelImage.IsValidIndex(GridIndex))
            {
                continue;
            }

            // 가운데 기준 실제 타일 번호 계산
            FolderNumber = TargetCenterFolder + (X - CenterIndex);
                

            FileNumber = TargetCenterFile + (Y - CenterIndex);
                

            FString TargetPath =
                FPaths::Combine(
                    ProjDir,
                    TEXT("IncheonLandFile/IncheonMichuolPNG"),
                    *FString::Printf(
                        TEXT("%d/%d/%d.png"),
                        ClampedScrollLevel,
                        FolderNumber,
                        FileNumber));

            UTexture2D* TileImage =
                LoadTextureFromFile(TargetPath);

            if (TileImage)
            {
                MapGridPanelImage[GridIndex]
                    ->SetBrushFromTexture(TileImage);
            }
        }
    }
}

void UMapViewer::MapRightButtonClick(const FVector2D& _MousePosition)
{
    SpawnMapPointMarker(_MousePosition);
}

void UMapViewer::MapLeftButtonClick()
{

}

void UMapViewer::PositionToImagePath()
{

}

void UMapViewer::ClearMapImage()
{
    for (int i = 0; i < MapGridPanelImage.Num(); i++) {
        if (MapGridPanelImage[i]) {
            MapGridPanelImage[i]->SetBrushFromTexture(nullptr);
        }
    }
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

        FVector LocalLocation; //이거 해야함
        ChangePointMarkerWorldPosition(LocalLocation , LocalScreenMousePostion);

        PointMarkerWidget->SetMarkerPosition(LocalLocation);

        UE_LOG(LogTemp, Error, TEXT("MapViewer GoalLocation : %s"), *LocalLocation.ToString());

        CityNewworkManager->SetPointMarkerArray(PointMarkerArray);
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
            WidgetLocationArray,
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

UTexture2D* UMapViewer::LoadTextureFromFile(FString _FilePath)
{
    if (!FPaths::FileExists(_FilePath))
    {
        return nullptr;
    }

    // 2. 바이너리 데이터를 파일로부터 읽어오기
    TArray<uint8> RawFileData;
    if (!FFileHelper::LoadFileToArray(RawFileData, *_FilePath))
    {
        return nullptr;
    }

    // 3. 이미지 에디팅/압축 해제를 위한 이미지 래퍼 모듈 가져오기
    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

    // PNG 형식으로 지정 (형식이 유동적이라면 DetectImageFormat을 써도 됩니다)
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
    {

        TArray<uint8> RawRGBA;
        if (ImageWrapper->GetRaw(ERGBFormat::RGBA, 8, RawRGBA))
        {
            int32 Width = ImageWrapper->GetWidth();
            int32 Height = ImageWrapper->GetHeight();

            // 1. [필수] FColor 배열의 크기를 픽셀 총 개수만큼 할당 (에러 해결 핵심)
            TArray<FColor> UncompressedRGBA;
            UncompressedRGBA.SetNumUninitialized(Width * Height);

            // 2. RawRGBA(uint8)의 메모리를 UncompressedRGBA(FColor)로 안전하게 복사
            FMemory::Memcpy(UncompressedRGBA.GetData(), RawRGBA.GetData(), RawRGBA.Num());

            FName UniqueTextureName = MakeUniqueObjectName(GetTransientPackage(), UTexture2D::StaticClass(), TEXT("RuntimeTexture"));

            // 3. 이제 크기가 채워진 UncompressedRGBA를 전달합니다.
            UTexture2D* NewTexture = FImageUtils::CreateTexture2D(
                Width,
                Height,
                UncompressedRGBA,
                GetTransientPackage(),
                UniqueTextureName.ToString(),
                RF_NoFlags,
                FCreateTexture2DParameters()
            );

            return NewTexture;
        }
    }

    return nullptr;
}

void UMapViewer::bMapMoveTrue()
{
    bMapMove = true;
}

void UMapViewer::UpdatePersonPosition(FVector PlayerLocation)
{
    if (!PersonMarker)
    {
        return;
    }

    UCanvasPanelSlot* CanvasSlot =
        Cast<UCanvasPanelSlot>(PersonMarker->Slot);

    if (!CanvasSlot)
    {
        return;
    }
    CommitPlayerLocation = PlayerLocation;

    if (NowScrollLevel == 13) {
        CommitPlayerLocation.X += 251000;
        CommitPlayerLocation.Y += 142000;
    }
    else if (NowScrollLevel == 14) {
        CommitPlayerLocation.X += 251000;
        CommitPlayerLocation.Y += 132000;
    }
    else if (NowScrollLevel == 15) {
        CommitPlayerLocation.X += 81000;
        CommitPlayerLocation.Y += 42000;
    }
    else if (NowScrollLevel == 16) {
        CommitPlayerLocation.X += 81000;
        CommitPlayerLocation.Y += 40000;
    }
    else if (NowScrollLevel == 17) {
        CommitPlayerLocation.X += 81000;
        CommitPlayerLocation.Y += 42000;
    }

    const double RatioX =
        FMath::Clamp(
            (CommitPlayerLocation.X - WorldMinX) /
            (WorldMaxX - WorldMinX),
            0.0,
            1.0);

    const double RatioY =
        FMath::Clamp(
            (CommitPlayerLocation.Y - WorldMinY) /
            (WorldMaxY - WorldMinY),
            0.0,
            1.0);

    if (!FolderFileBaseMap.Contains(NowScrollLevel))
    {
        return;
    }

    const FFolderFileStartEndBase& ZoomInfo =
        FolderFileBaseMap[NowScrollLevel];

    const int32 StartFolder =
        ZoomInfo.BaseStartFolder;

    const int32 EndFolder =
        ZoomInfo.BaseEndFolder;

    const int32 StartFile =
        ZoomInfo.BaseStartFile;

    const int32 EndFile =
        ZoomInfo.BaseEndFile;

    ////------------------------------------
    //// 전체 타일 공간에서 위치 계산
    ////------------------------------------

    //const double FilePos = StartFile + RatioX * (EndFile - StartFile);

    //const double FolderPos = StartFolder + RatioY * (EndFolder - StartFolder);

    // 가로가 폴더이므로 RatioX(월드 좌우)를 폴더에 곱합니다.
    const double FolderPos = StartFolder + RatioX * (EndFolder - StartFolder);

    // 세로가 파일이므로 RatioY(월드 상하)를 파일에 곱합니다.
    const double FilePos = StartFile + RatioY * (EndFile - StartFile);

    //------------------------------------
    // 현재 화면에 보이는 범위
    //------------------------------------

    const int32 CenterIndex =
        MaxImageCountInFolder / 2;

    const int32 VisibleMinFolder =
        TargetCenterFolder - CenterIndex;

    const int32 VisibleMaxFolder =
        TargetCenterFolder + CenterIndex;

    const int32 VisibleMinFile =
        TargetCenterFile - CenterIndex;

    const int32 VisibleMaxFile =
        TargetCenterFile + CenterIndex;

    //VisibleMinFolder,VisibleMaxFolder,VisibleMinFile,VisibleMaxFile 까진 정답

    //------------------------------------
    // 화면 밖이면 숨김
    //------------------------------------

    if (FolderPos < VisibleMinFolder ||
        FolderPos > VisibleMaxFolder ||
        FilePos < VisibleMinFile ||
        FilePos > VisibleMaxFile)
    {
        PersonMarker->SetVisibility(
            ESlateVisibility::Hidden);

        return;
    }

    PersonMarker->SetVisibility(
        ESlateVisibility::Visible);

    const double LocalFolder = FolderPos - VisibleMinFolder;
    const double LocalFile = FilePos - VisibleMinFile;

    //------------------------------------
    // 타일 좌표 -> 픽셀 좌표
    //------------------------------------

    ////#반대로 움직이는 버전
    double PixelX = LocalFolder * UITimeImageSize;
    double PixelY = LocalFile * UITimeImageSize;

    //------------------------------------
    // 마커 중앙 정렬
    //------------------------------------

    const FVector2D MarkerSize =
        PersonMarker->GetDesiredSize();

    CanvasSlot->SetPosition(
        FVector2D(
            PixelX,
            PixelY));

    UE_LOG(LogTemp, Warning,
        TEXT("World=(%f,%f) Ratio=(%f,%f) FilePos=%f FolderPos=%f"),
        CommitPlayerLocation.X,
        CommitPlayerLocation.Y,
        RatioX,
        RatioY,
        FilePos,
        FolderPos);

}



void UMapViewer::SetLocalMapSize(float _WorldMinX, float _WorldMaxX, float _WorldMinY, float _WorldMaxY)
{
    UE_LOG(LogTemp, Error, TEXT("WorldMinX : %f, WorldMinY : %f, WorldMaxX : %f, WorldMaxY : %f"), WorldMinX, WorldMinY, WorldMaxX, WorldMaxY);

    WorldMinX = _WorldMinX;
    WorldMinY = _WorldMinY;
    WorldMaxX = _WorldMaxX;
    WorldMaxY = _WorldMaxY;

    float XMid = (WorldMinX + WorldMaxX) * 0.5f;
    float YMid = (WorldMinY + WorldMaxY) * 0.5f;

    UE_LOG(LogTemp, Error, TEXT("%f, %f"), XMid, YMid);

    TArray<FVector2D> Base;
    Base.Add(FVector2D(667259, -178848));
    Base.Add(FVector2D(665777, -180244));
    Base.Add(FVector2D(567332, -285856));
    Base.Add(FVector2D(506587, -286205));
    Base.Add(FVector2D(506379, -286221));

    for (int32 Zoom = 13; Zoom <= 17; Zoom++)
    {
        const int32 Index = Zoom - 13;

        FVector2D Offset;
        Offset.X = XMid - Base[Index].X;
        Offset.Y = YMid - Base[Index].Y;

        ZoomCenterOffset.Add(Zoom, Offset);
    }
    //13
    //World = X = 667259.562 Y = -178848

    //14
    //World = X = 665777.250 Y = -180244.641

    //15
    //World = X = 567332.188 Y = -285856.812

    //16
    //World = X = 506587.469 Y = -286205.906

    //17
    //World = X = 506379.781 Y = -286221.688
}
