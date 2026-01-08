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
        FLinearColor Color = LinearBitmap[i];

        float Alpha = FMath::Clamp(Color.A, 0.0f, 1.0f);

        FLinearColor StraightColor = Color;

        if (Alpha > 0.0001f)
        {
            StraightColor.R /= Alpha;
            StraightColor.G /= Alpha;
            StraightColor.B /= Alpha;
        }
        else
        {
            StraightColor.R = 0;
            StraightColor.G = 0;
            StraightColor.B = 0;
        }

        StraightColor.A = Alpha;

        Bitmap[i] = StraightColor.ToFColor(false);
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

    UE_LOG(LogTemp, Log, TEXT("Saved PNG successfully: %s"), *FilePath);
}
