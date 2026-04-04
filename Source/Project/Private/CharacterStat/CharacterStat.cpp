// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStat.h"
#include "ProjectPlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterStat/StatAnimInstance/StatAnimInstance.h"
#include "ProjectCharacter.h"

//Server
#include "JsonObjectConverter.h"

//Widget
#include "CharacterStat/CharacterStatWidget/ChildWidget/SkillWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/MainWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/RelicWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/StarWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ACharacterStat::SetBeginServerData()
{
    // Http 모듈 싱글톤 인스턴스 가져오기
    FHttpModule* Http = &FHttpModule::Get();

    if (Http)
    {
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

        // ... 이후 URL 세팅 및 요청 로직
        Request->SetURL(TEXT("http://localhost:3000/api/skills/player_01"));
        Request->SetVerb(TEXT("GET"));
        // BindUObject를 사용하고, 인자가 포함된 함수를 연결합니다.
        Request->OnProcessRequestComplete().BindUObject(this, &ACharacterStat::OnCharacterDataReceived);

        Request->ProcessRequest();
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("NO HTTP"));
    }
}

void ACharacterStat::OnCharacterDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    //if (bWasSuccessful && Response.IsValid())
    //{
    //    //UE_LOG(LogTemp, Log, TEXT("Success: %s"), *Response->GetContentAsString());
    //}

    //if (!SkillWidget || !MainWidget || !StarWidget || !RelicWidget) {
    //    UE_LOG(LogTemp, Error, TEXT("No Widget"), *Response->GetContentAsString());
    //    return;
    //}

    //// 2. 서버에서 보낸 문자열 가져오기
    //FString JsonString = Response->GetContentAsString();
    ////UE_LOG(LogTemp, Log, TEXT("Server JSON: %s"), *JsonString);

    //// 3. JSON 문자열을 구조체로 변환 (역직렬화)
    //FCharacterDataWrapper RawData;

    //// TJsonReader를 생성하여 파싱 준비
    //TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    //TSharedPtr<FJsonObject> JsonObject;

    //if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    //{
    //    // JsonObjectConverter를 사용하면 한 줄로 구조체에 값이 담깁니다.
    //    if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), FCharacterDataWrapper::StaticStruct(), &RawData, 0, 0))
    //    {
    //        MainWidget->UpdateWithServerData(RawData.Maininfo);

    //        SkillWidget->UpdateWithServerData(RawData.SkillInfo);

    //        StarWidget->UpdateWithServerData(RawData.StarInfo);

    //        RelicWidget->UpdateWithServerData(RawData.Relicinfo);
    //    }
    //    else {
    //        UE_LOG(LogTemp, Error, TEXT("Faild JsonObjectToUStruct!"), *JsonString);
    //    }
    //}
    //else {
    //    UE_LOG(LogTemp, Error, TEXT("Faild Deserialize!"), *JsonString);
    //}
    if (!bWasSuccessful || !Response.IsValid()) return;

    FString JsonString = Response->GetContentAsString();
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject;

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FCharacterDataWrapper RawData;

        // [중요] Postman 결과처럼 데이터가 바로 들어오므로, 
        // 추가적인 Field 체크 없이 바로 UStruct로 변환합니다.
        if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), FCharacterDataWrapper::StaticStruct(), &RawData, 0, 0))
        {
            if (SkillWidget) SkillWidget->UpdateWithServerData(RawData.SkillInfo);
            if (StarWidget)  StarWidget->UpdateWithServerData(RawData.StarInfo);
            if (RelicWidget) RelicWidget->UpdateWithServerData(RawData.Relicinfo);
            if (MainWidget)  MainWidget->UpdateWithServerData(RawData.Maininfo);

            UE_LOG(LogTemp, Log, TEXT("Successfully updated data for: %s"), *RawData.CharacterName);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed JsonObjectToUStruct!"));
        }
    }

}

UMainWidget* ACharacterStat::GetMainWidget()
{
    return MainWidget;
}

void ACharacterStat::FindWidgetClass(USkillWidget* Skill, UMainWidget* Main, URelicWidget* Relic, UStarWidget* Star)
{
    SkillWidget = Skill;
    MainWidget = Main;
    RelicWidget = Relic;
    StarWidget = Star;

    SetBeginServerData();
}

void ACharacterStat::SetMainStat(FMaininfo value)
{
    MainStat.HP += value.HP;
    MainStat.Defence += value.Defence;
    MainStat.Attack += value.Attack;
    MainStat.Force += value.Force;
    MainStat.Critical += value.Critical;
    MainStat.CriticalDamage += value.CriticalDamage;

}

FMaininfo ACharacterStat::GetMainStat()
{
    return MainStat;
}

// Sets default values
ACharacterStat::ACharacterStat()
{
	PrimaryActorTick.bCanEverTick = true;

    // 2. 카메라 생성 및 카메라 붐 끝에 부착
    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(RootComponent);

    RelicCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("RelicCamera"));
    RelicCamera->SetupAttachment(RootComponent);

    SkillCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SkillCamera"));
    SkillCamera->SetupAttachment(RootComponent);

    StarCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("StarCamera"));
    StarCamera->SetupAttachment(RootComponent);

    MainCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
    MainCamera->SetupAttachment(RootComponent);
    
}

// Called when the game starts or when spawned
void ACharacterStat::BeginPlay()
{
	Super::BeginPlay();
	
    TargetCam = MainCamera;

    // 시작 시점의 위치를 기본 카메라 위치로 초기화
    if (ViewCamera)
    {
        TargetLocation = ViewCamera->GetRelativeLocation();
        TargetRotation = ViewCamera->GetRelativeRotation();
    }

    RelicCamera->SetActive(false);
    MainCamera->SetActive(false);
    SkillCamera->SetActive(false);
    StarCamera->SetActive(false);
    ViewCamera->SetActive(true);

    StatAnimInstance = Cast<UStatAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void ACharacterStat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLoc = FMath::VInterpTo(ViewCamera->GetRelativeLocation(), TargetLocation, DeltaTime, CameraInterpSpeed);
    FRotator NewRot = FMath::RInterpTo(ViewCamera->GetRelativeRotation(), TargetRotation, DeltaTime, CameraInterpSpeed);

    ViewCamera->SetRelativeLocation(NewLoc);
    ViewCamera->SetRelativeRotation(NewRot);

}

void ACharacterStat::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACharacterStat::SwitchCameraComponent(ECharacterMenuState MenuState)
{

    if (CurMenuState == MenuState) return;

    switch (MenuState)
    {
    case ECharacterMenuState::Relic:
        TargetCam = RelicCamera;
        break;
    case ECharacterMenuState::Skill:
        TargetCam = SkillCamera;
        break;
    case ECharacterMenuState::Star:
        TargetCam = StarCamera;
        break;
    case ECharacterMenuState::Main:
        TargetCam = MainCamera;
        break;
    }

    CurMenuState = MenuState;

    PlayAnimation(MenuState);

    if (TargetCam)
    {
        // 목표 지점을 해당 카메라의 '상대 좌표'로 설정합니다.
        TargetLocation = TargetCam->GetRelativeLocation();
        TargetRotation = TargetCam->GetRelativeRotation();
    }
}

void ACharacterStat::SetStatFollowCamera(UCameraComponent* value)
{
    FollowCamera = value;
}

void ACharacterStat::ChangeCamera()
{
    AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());

    PC->SetViewTargetWithBlend(this, 0.f, VTBlend_Cubic);

    TargetCam = MainCamera;

    TargetLocation = TargetCam->GetRelativeLocation();
    TargetRotation = TargetCam->GetRelativeRotation();

    ViewCamera->SetRelativeLocation(TargetLocation);
    ViewCamera->SetRelativeRotation(TargetRotation);

    // 1. 카메라 전환 로직 (기존 코드)
    PC->SetViewTargetWithBlend(this, 0.f, VTBlend_Cubic);

    // 2. 마우스 커서 활성화
    PC->bShowMouseCursor = true;

    // 3. 입력 모드 설정 (UI와 게임 모두 상호작용 가능하게 설정)
    // 게임 조작을 완전히 막으려면 FInputModeUIOnly를 사용하세요.
    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    PC->SetInputMode(InputMode);

    ViewCamera->SetActive(true);

    SwitchCameraComponent(ECharacterMenuState::Main); //Main으로 초기화

}

void ACharacterStat::RestoreCamera()
{
    AProjectPlayerController* PC = Cast<AProjectPlayerController>(GetWorld()->GetFirstPlayerController());

    if (PC)
    {
        
        ViewCamera->SetActive(false);
        // 1. 다시 플레이어 캐릭터(this)를 바라보게 설정 (0.5초 동안 부드럽게 복귀)
        // 즉시 돌아가고 싶다면 0.f를 사용하세요.
        PC->SetViewTargetWithBlend(Cast<AProjectCharacter>(PC->GetPawn()), 0.0f, VTBlend_Cubic);

        // 2. 마우스 커서 숨기기
        PC->bShowMouseCursor = false;

        // 3. 입력 모드를 다시 게임 전용으로 변경
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }

    if (FollowCamera) {
        FollowCamera->SetActive(true);
    }
}

void ACharacterStat::SetCurrentCharacterKey(FName value)
{
    CurrentCharacterKey = value;
}

FName ACharacterStat::GetCurrentCharacterKey()
{
    return CurrentCharacterKey;
}

void ACharacterStat::PlayAnimation(ECharacterMenuState value)
{
    //Y 수직 X 수평
    //1 main 정면
    //2 relic 오른쪽 위
    //3 skill 왼쪽 아래
    //4 star 왼쪽 위

    //Relic	Skill

    //Main	Star

    switch (value)
    {
    case ECharacterMenuState::Main:
        StatAnimInstance->SetBlendSpace(0, 0);
        break;
    case ECharacterMenuState::Relic:
        StatAnimInstance->SetBlendSpace(1, 0);
        break;
    case ECharacterMenuState::Skill:
        StatAnimInstance->SetBlendSpace(0, 1);
        break;
    case ECharacterMenuState::Star:
        StatAnimInstance->SetBlendSpace(1, 1);
        break;
    default:
        break;
    }

}

// .cpp 파일 구현
void ACharacterStat::SendSkillUpgradeToServer(const FSkillInfo& NewSkillInfo)
{
    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) return;

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    // 1. 서버 URL 설정 (우리가 만든 Node.js 주소)
    // player_01 부분은 나중에 변수로 처리하면 좋습니다.
    Request->SetURL(TEXT("http://localhost:3000/api/skills/player_01"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    // 2. 구조체를 JSON 문자열로 변환 (직렬화)
    FString JsonString;
    if (FJsonObjectConverter::UStructToJsonObjectString(NewSkillInfo, JsonString))
    {
        Request->SetContentAsString(JsonString);

        // 3. 응답 처리 바인딩
        Request->OnProcessRequestComplete().BindUObject(this, &ACharacterStat::OnSkillUpgradeResponse);

        Request->ProcessRequest();
        UE_LOG(LogTemp, Log, TEXT("Sending Upgrade Data: %s"), *JsonString);
    }
}

void ACharacterStat::OnSkillUpgradeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("Upgrade Success! Server Response: %s"), *Response->GetContentAsString());
        // 성공 시 UI를 다시 갱신하거나 효과음을 재생하는 로직을 여기에 넣으세요.



    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Upgrade Failed!"));
    }
}