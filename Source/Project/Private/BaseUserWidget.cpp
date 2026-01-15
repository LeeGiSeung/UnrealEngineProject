// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseUserWidget.h"

#include "Engine/CanvasRenderTarget2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RenderUtils.h"
#include "ImageUtils.h" // 일부 기능용
#include "IImageWrapperModule.h"
#include "IImageWrapper.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Colorenum/Colorenum.h"

void UBaseUserWidget::RecordPosition(FVector2D Pos)
{
    DrawPositions.Add(Pos);

    //UE_LOG(LogTemp, Warning, TEXT("add position : %s"), *Pos.ToString());
    //UE_LOG(LogTemp, Warning, TEXT("position Num : %d"), DrawPositions.Num());
}

void UBaseUserWidget::SaveCanvasRenderTargetToPNG(UTextureRenderTarget2D* Canvas, const FString& FilePath, const FString& FileName)
{
    if (!Canvas)
    {
        UE_LOG(LogTemp, Warning, TEXT("Canvas is null"));
        return;
    }

    FRenderTarget* RTResource = Canvas->GameThread_GetRenderTargetResource();
    if (!RTResource)
    {
        UE_LOG(LogTemp, Warning, TEXT("Render target resource is null"));
        return;
    }

    TArray<FLinearColor> LinearBitmap;
    if (!RTResource->ReadLinearColorPixels(LinearBitmap))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to read pixels from render target"));
        return;
    }

    int32 Width = Canvas->SizeX;
    int32 Height = Canvas->SizeY;
    if (Width == 0 || Height == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid render target size"));
        return;
    }

    TArray<FColor> Bitmap;
    Bitmap.SetNum(LinearBitmap.Num());
    

    for (int32 i = 0; i < LinearBitmap.Num(); i++)
    {
        const FLinearColor& Src = LinearBitmap[i];

        // 알파 무시, RGB만 그대로
        FLinearColor Out;
        Out.R = Src.R;
        Out.G = Src.G;
        Out.B = Src.B;
        Out.A = 1.0f; // 강제로 불투명

        Bitmap[i] = Out.ToFColor(false);
    }

    IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
    TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

    if (!ImageWrapper->SetRaw(Bitmap.GetData(), Bitmap.Num() * sizeof(FColor), Width, Height, ERGBFormat::BGRA, 8))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to SetRaw for PNG encoding"));
        return;
    }

    const TArray64<uint8>& PNGData = ImageWrapper->GetCompressed(100);

    FString Name = FilePath + FileName;

    if (!FFileHelper::SaveArrayToFile(PNGData, *FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to save PNG to file %s"), *FilePath);
        return;
    }

    //UE_LOG(LogTemp, Log, TEXT("Saved PNG successfully: %s"), *FilePath);
}

void UBaseUserWidget::FinishDrawing()
{
    OnDrawFinished.Broadcast();

}

void UBaseUserWidget::ChangeCenterCursorColor(EColor _color)
{
    FLinearColor CenterColor;

    switch (_color)
    {
    case EColor::RED:
        CenterColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
        break;
    case EColor::BLUE:
        CenterColor = FLinearColor(0.f, 0.f, 1.f, 1.f);
        break;
    case EColor::YELLOW:
        CenterColor = FLinearColor(1.f, 1.f, 0.f, 1.f);
        break;
    case EColor::GREEN:
        CenterColor = FLinearColor(0.f, 1.f, 0.f, 1.f);
        break;
    default:
        break;
    }

    if (CenterCursorMaterial)
    {
        UE_LOG(LogTemp, Warning, TEXT("MATEaaaaaR"));
        CenterCursorMaterial->SetVectorParameterValue(
            FName("Color"),                 // ParameterName
            CenterColor
        );
    }
}

void UBaseUserWidget::ChangeBrushStartColor(EColor _color, UMaterialInstanceDynamic* _Brush)
{
    FLinearColor CenterColor;

    switch (_color)
    {
    case EColor::RED:
        CenterColor = FLinearColor(1.f, 0.f, 0.f, 1.f);
        break;
    case EColor::BLUE:
        CenterColor = FLinearColor(0.f, 0.f, 1.f, 1.f);
        break;
    case EColor::YELLOW:
        CenterColor = FLinearColor(1.f, 1.f, 0.f, 1.f);
        break;
    case EColor::GREEN:
        CenterColor = FLinearColor(0.f, 1.f, 0.f, 1.f);
        break;
    default:
        break;
    }

    if (_Brush)
    {
        UE_LOG(LogTemp, Warning, TEXT("MATEaaaaaR"));
        _Brush->SetVectorParameterValue(
            FName("Color"),                 // ParameterName
            CenterColor
        );
    }
}

