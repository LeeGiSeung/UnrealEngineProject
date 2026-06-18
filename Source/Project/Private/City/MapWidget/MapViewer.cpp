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

//Image
#include "ImageUtils.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"

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
    if (NowScollLevel + 1 > maxScollLevel) return;
    NowScollLevel++;

    UE_LOG(LogTemp, Error, TEXT("NowScollLevel : %d"), NowScollLevel);
    
    ChangeMapImage();
}

void UMapViewer::ScroolDown()
{
    if (NowScollLevel - 1 < minScollLevel) return;
    NowScollLevel--;

    UE_LOG(LogTemp, Error, TEXT("NowScollLevel : %d"), NowScollLevel);

    ChangeMapImage();
}

void UMapViewer::MapMove(FVector2D value)
{
	
}

void UMapViewer::ChangeMapImage()
{
    ClearMapImage();

    const double TileSize = 256.0;

    // 1. 요구사항에 맞춘 레벨 13 기준 원점 데이터 설정
    const int32 BaseZoom = 13;
    const int32 BaseStartX = 6977; // 시작 폴더 번호 변경
    const int32 BaseStartY = 3174; // 시작 PNG 이름 변경

    // 2. NowScollLevel 변수를 기반으로 현재 레벨 판단 및 13~17 레벨 범위 제한 (안전장치)
    int32 ClampedScrollLevel = FMath::Clamp(NowScollLevel, 13, 17);

    // 줌 레벨 차이 계산
    int32 ZoomDiff = ClampedScrollLevel - BaseZoom;

    // 2의 거듭제곱 배수 구하기 (레벨이 1 오를 때마다 2배씩 증가)
    int32 ScaleMultiplier = (FMath::Pow(2.0, ZoomDiff));

    UE_LOG(LogTemp, Error, TEXT("ScaleMultiplier : %d"), ScaleMultiplier);

    // 현재 줌 레벨에 맞는 진짜 시작 타일 번호 자동 계산
    int32 BaseFolderStart = BaseStartX * ScaleMultiplier;
    int32 BaseFileStart = BaseStartY * ScaleMultiplier;

    int32 CurrentFoloderStart = BaseFolderStart;
    int32 CurrentFileStart = BaseFileStart;

    FString ProjDir = FPaths::ProjectDir();

    FString BaseFolderPath = FPaths::Combine(ProjDir, TEXT("IncheonLandFile/IncheonMichuolPNG"),
        *FString::Printf(TEXT("%d/%d"), ClampedScrollLevel, BaseFolderStart));

    TArray<FString> FoundFiles;
    IFileManager::Get().FindFiles(FoundFiles, *BaseFolderPath, TEXT("*.png"));

    MaxImageCountInFolder = FoundFiles.Num();
    int32 CurImageCountInFolder = 0;

    bool flag = true;

    // 4x4 그리드에서 세로 방향으로 순회하기 위한 이중 루프
    for (int32 X = 0; X < 4; ++X)
    {
        if (!flag) break; //만약폴더 번호가 넘으면 return 
        for (int32 Y = 0; Y < 4; ++Y)
        {
            int32 GridIndex = (Y * 4) + X;

            if (MapGridPanelImage.IsValidIndex(GridIndex))
            {
                if (CurImageCountInFolder >= MaxImageCountInFolder) {
                    CurImageCountInFolder = 0;
                    CurrentFoloderStart++; 
                    CurrentFileStart = BaseFileStart;
                    if (CurrentFoloderStart >= BaseFolderStart * 2)
                    {
                        flag = false;
                        break;
                    }

                }
                FString TargetPath = FPaths::Combine(ProjDir, TEXT("IncheonLandFile/IncheonMichuolPNG"),
                    *FString::Printf(TEXT("%d/%d/%d.png"), ClampedScrollLevel, CurrentFoloderStart, CurrentFileStart));

                UE_LOG(LogTemp, Error, TEXT("FilePath : %s"), *TargetPath);

                UTexture2D* TileImage = LoadTextureFromFile(TargetPath);

                if (!TileImage) {
                    //UE_LOG(LogTemp, Error, TEXT("NO TileImage"));
                    continue;
                }

                MapGridPanelImage[GridIndex]->SetBrushFromTexture(TileImage);
                CurrentFileStart++;
                CurImageCountInFolder++;
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

            // 3. 이제 크기가 채워진 UncompressedRGBA를 전달합니다.
            UTexture2D* NewTexture = FImageUtils::CreateTexture2D(
                Width,
                Height,
                UncompressedRGBA,
                GetTransientPackage(),
                TEXT("RuntimeSpawnedTexture"),
                RF_NoFlags,
                FCreateTexture2DParameters()
            );

            return NewTexture;
        }
    }

    return nullptr;
}




