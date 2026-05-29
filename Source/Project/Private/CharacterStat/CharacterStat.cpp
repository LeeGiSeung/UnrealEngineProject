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

//WebSocket
#include "SocketIOClientComponent.h"
#include "IWebSocket.h" // 모듈 헤더
#include "WebSocketsModule.h"

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

    //SocketIOComponent = CreateDefaultSubobject<USocketIOClientComponent>(TEXT("SocketIOComponent"));
    ////SocketIOComponent->AddressAndPort = TEXT("http://localhost:3000"); 
    //SocketIOComponent->bShouldAutoConnect = true; // 자동 연결 활성화

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

    

    //if (SocketIOComponent) {
    //    SocketIOComponent->OnNativeEvent(TEXT("data_changed"), [](const FString& Event, const TSharedPtr<FJsonValue>& Message)
    //    {
    //        //Called when the event is received. We can e.g. log what we got
    //        UE_LOG(LogTemp, Log, TEXT("Received: %s"), *USIOJConvert::ToJsonString(Message));
    //    });
    //}

    //SocketIOComponent->EmitNative(TEXT("data_changed"), TEXT("hi"));

    NativeSocket = FWebSocketsModule::Get().CreateWebSocket("ws://127.0.0.1:3000");

    if (!NativeSocket) {
        UE_LOG(LogTemp, Error, TEXT("no NativeSocket"));
        return;
    }

    // 메시지를 받았을 때의 처리
    NativeSocket->OnMessage().AddLambda([this](const FString& Message) {
        if (Message == "REFRESH_STATS")
        {
            AsyncTask(ENamedThreads::GameThread, [this]() {
                SetBeginServerData();
                });

        }
        });

    NativeSocket->Connect();

}

void ACharacterStat::SetBeginServerData()
{
    FHttpModule* Http = &FHttpModule::Get();

    if (MainWidget) {
        MainWidget->ResetSubinfo();
    }
    if (Http)
    {
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

        Request->SetURL(TEXT("http://localhost:3000/api/skills/player_01"));
        Request->SetVerb(TEXT("GET"));
        Request->OnProcessRequestComplete().BindUObject(this, &ACharacterStat::OnCharacterDataReceived);

        Request->ProcessRequest();
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("NO HTTP"));
    }
}

void ACharacterStat::ResetMainStat()
{
    MainStat.HP = 0.f;
    MainStat.Defence = 0.f;
    MainStat.Attack = 0.f;
    MainStat.Force = 0.f;
    MainStat.Critical = 0.f;
    MainStat.CriticalDamage = 0.f;
    MainStat.Level = 0.f;
    MainStat.LevelEXP = 0.f;
}

void ACharacterStat::OnCharacterDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    //Server로부터 데이터 업데이트
    if (!bWasSuccessful || !Response.IsValid()) return;

    FString JsonString = Response->GetContentAsString();
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject;

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        FCharacterDataWrapper RawData;

        if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), FCharacterDataWrapper::StaticStruct(), &RawData, 0, 0))
        {
            if (SkillWidget) SkillWidget->UpdateWithServerData(RawData.SkillInfo);
            if (StarWidget)  StarWidget->UpdateWithServerData(RawData.StarInfo);
            if (RelicWidget) RelicWidget->UpdateWithServerData(RawData.Relicinfo);
            if (MainWidget)  MainWidget->UpdateWithServerData(RawData.Maininfo);
            SetMainStat(RawData.Maininfo);

            //UE_LOG(LogTemp, Log, TEXT("Successfully updated data for: %s"), *RawData.CharacterName);
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

    //MainStat.HP += value.HP;
    //MainStat.Defence += value.Defence;
    //MainStat.Attack += value.Attack;
    //MainStat.Force += value.Force;
    //MainStat.Critical += value.Critical;
    //MainStat.CriticalDamage += value.CriticalDamage;
    //MainStat.Level += value.Level;
    //MainStat.LevelEXP += value.LevelEXP;
}

FMaininfo ACharacterStat::GetMainStat()
{
    return MainStat;
}

// Called every frame
void ACharacterStat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    FVector NewLoc = FMath::VInterpTo(ViewCamera->GetRelativeLocation(), TargetLocation, DeltaTime, CameraInterpSpeed);
    FRotator NewRot = FMath::RInterpTo(ViewCamera->GetRelativeRotation(), TargetRotation, DeltaTime, CameraInterpSpeed);

    ViewCamera->SetRelativeLocation(NewLoc);
    ViewCamera->SetRelativeRotation(NewRot);

    //UE_LOG(LogTemp, Error, TEXT("HP : %f"), MainStat.HP);
    

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
void ACharacterStat::SendSkillUpgradeToServer()
{

    FHttpModule* Http = &FHttpModule::Get();
    if (!Http) return;

    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

    // 1. 통합 데이터 구조체 생성 및 데이터 채우기
    FCharacterDataWrapper TotalData;

    // 요청하신 대로 고정값 세팅
    TotalData.PlayerId = TEXT("player_01");
    TotalData.CharacterId = TEXT("CH_V_001");
    TotalData.CharacterName = TEXT("Aria");

    // 각 위젯으로부터 개별 구조체 데이터 수집
    if (SkillWidget) TotalData.SkillInfo = SkillWidget->GetSkillInfo();
    if (RelicWidget) TotalData.Relicinfo = RelicWidget->GetRelicInfo();
    if (StarWidget)  TotalData.StarInfo = StarWidget->GetStarInfo();
    if (MainWidget)  TotalData.Maininfo = MainWidget->GetMainInfo();

    TotalData.Maininfo = GetMainStat();

    Request->SetURL(TEXT("http://localhost:3000/api/skills/player_01"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    FString JsonString;
    if (FJsonObjectConverter::UStructToJsonObjectString(TotalData, JsonString))
    {
        Request->SetContentAsString(JsonString);

        // 응답 처리 바인딩
        Request->OnProcessRequestComplete().BindUObject(this, &ACharacterStat::OnSkillUpgradeResponse);

        Request->ProcessRequest();

        // 로그로 전체 데이터 확인
        //UE_LOG(LogTemp, Log, TEXT("Sending Full Character Data: %s"), *JsonString);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to serialize CharacterDataWrapper!"));
    }
}

void ACharacterStat::OnSkillUpgradeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        //UE_LOG(LogTemp, Log, TEXT("Upgrade Success! Server Response: %s"), *Response->GetContentAsString());
        // 성공 시 UI를 다시 갱신하거나 효과음을 재생하는 로직을 여기에 넣으세요.



    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Upgrade Failed!"));
    }
}