// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/SkillWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/SillButtonWidget/SkillButtonWidget.h"

//#Server
#include "JsonObjectConverter.h"

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AutoAttackImage || !EAttackImage || !RAttackImage || !PAttackImage) return;

	if (!AutoAttackWidget || !EAttackWidget || !RAttackWidget || !PAttackWidget) return;

	AutoAttackWidget->SettingSkillImage(AutoAttackImage, this);
	
	EAttackWidget->SettingSkillImage(EAttackImage, this);

	RAttackWidget->SettingSkillImage(RAttackImage, this);

	PAttackWidget->SettingSkillImage(PAttackImage, this);

    RequestSkillDataFromServer();

}

void USkillWidget::UpdateCharacterData()
{
	Super::UpdateCharacterData();

	if (AutoAttackWidget) AutoAttackWidget->SettingSkillLevel(SkillData.AutoAttackLevel, this);
	if (EAttackWidget)    EAttackWidget->SettingSkillLevel(SkillData.EAttackLevel, this);
	if (RAttackWidget)    RAttackWidget->SettingSkillLevel(SkillData.RAttackLevel, this);
	if (PAttackWidget)    PAttackWidget->SettingSkillLevel(SkillData.PAttackLevel, this);
}

void USkillWidget::SkillToMainStat(FSkillInfo Data)
{

}

void USkillWidget::SkillLevelUp()
{
    
}

void USkillWidget::RecoardSkillButtonWidget(USkillButtonWidget* value)
{
    LastRecoardSkillButtonWidget = value;
}

void USkillWidget::OnSkillDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    return;

    // 1. 통신 성공 여부 확인
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Server Faild!"));
        return;
    }

    // 2. 서버에서 보낸 문자열 가져오기
    FString JsonString = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("Server JSON: %s"), *JsonString);

    // 3. JSON 문자열을 구조체로 변환 (역직렬화)
    FSkillInfo RawData;

    // TJsonReader를 생성하여 파싱 준비
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    TSharedPtr<FJsonObject> JsonObject;

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        // JsonObjectConverter를 사용하면 한 줄로 구조체에 값이 담깁니다.
        if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), FSkillInfo::StaticStruct(), &RawData, 0, 0))
        {
            // 4. 파싱된 데이터를 위젯에 반영
            UpdateWithServerData(RawData);
        }
        else {
            UE_LOG(LogTemp, Error, TEXT("Faild JsonObjectToUStruct!"), *JsonString);
        }
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("Faild Deserialize!"), *JsonString);
    }
}

void USkillWidget::UpdateWithServerData(const FSkillInfo& Data)
{
    if (AutoAttackWidget) AutoAttackWidget->SettingSkillLevel(Data.AutoAttackLevel, this);
    if (EAttackWidget)    EAttackWidget->SettingSkillLevel(Data.EAttackLevel, this);
    if (RAttackWidget)    RAttackWidget->SettingSkillLevel(Data.RAttackLevel, this);
    if (PAttackWidget)    PAttackWidget->SettingSkillLevel(Data.PAttackLevel, this);

    Auto_0->SettingNodeImage(Data.AutoAttackNode_0, this);
    Auto_1->SettingNodeImage(Data.AutoAttackNode_1, this);
    Auto_2->SettingNodeImage(Data.AutoAttackNode_2, this);

    E_0->SettingNodeImage(Data.EAttackkNode_0, this);
    E_1->SettingNodeImage(Data.EAttackNode_1, this);
    E_2->SettingNodeImage(Data.EAttackNode_2, this);

    P_0->SettingNodeImage(Data.PAttackNode_0, this);
    P_1->SettingNodeImage(Data.PAttackNode_1, this);
    P_2->SettingNodeImage(Data.PAttackNode_2, this);

    R_0->SettingNodeImage(Data.RAttackNode_0, this);
    R_1->SettingNodeImage(Data.RAttackNode_0, this);
    R_2->SettingNodeImage(Data.RAttackNode_0, this);
    
}

void USkillWidget::RequestSkillDataFromServer()
{

    // Http 모듈 싱글톤 인스턴스 가져오기
    FHttpModule* Http = &FHttpModule::Get();

    if (Http)
    {
        TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();

        // ... 이후 URL 세팅 및 요청 로직
        Request->SetURL(TEXT("https://a57b9c47-8b8e-42fc-8204-e424bd476fe0.mock.pstmn.io/skills"));
        Request->SetVerb(TEXT("GET"));
        Request->OnProcessRequestComplete().BindUObject(this, &USkillWidget::OnSkillDataReceived);
        Request->ProcessRequest();
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("NO HTTP"));
    }
}

void USkillWidget::TestGet()
{
    RequestSkillDataFromServer();
}
