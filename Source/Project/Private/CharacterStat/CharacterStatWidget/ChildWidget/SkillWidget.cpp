// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/CharacterStatWidget/ChildWidget/SkillWidget.h"
#include "CharacterStat/CharacterStatWidget/ChildWidget/SillButtonWidget/SkillButtonWidget.h"
#include "CharacterStat/CharacterStat.h"

//#Server
#include "JsonObjectConverter.h"

void USkillWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!AutoAttackImage || !EAttackImage || !RAttackImage || !PAttackImage) return;

	if (!AutoAttackWidget || !EAttackWidget || !RAttackWidget || !PAttackWidget) return;

	AutoAttackWidget->SettingSkillImage(AutoAttackImage);
	
	EAttackWidget->SettingSkillImage(EAttackImage);

	RAttackWidget->SettingSkillImage(RAttackImage);

	PAttackWidget->SettingSkillImage(PAttackImage);

    AutoAttackWidget->SettingSkillWidget(this);
    EAttackWidget->SettingSkillWidget(this);
    RAttackWidget->SettingSkillWidget(this);
    PAttackWidget->SettingSkillWidget(this);

    Auto_0->SettingSkillWidget(this);
    Auto_1->SettingSkillWidget(this);
    Auto_2->SettingSkillWidget(this);

    E_0->SettingSkillWidget(this);
    E_1->SettingSkillWidget(this);
    E_2->SettingSkillWidget(this);

    P_0->SettingSkillWidget(this);
    P_1->SettingSkillWidget(this);
    P_2->SettingSkillWidget(this);

    R_0->SettingSkillWidget(this);
    R_1->SettingSkillWidget(this);
    R_2->SettingSkillWidget(this);

    AutoAttackWidget->SetSkillDataKey(TEXT("AutoAttackLevel"));
    Auto_0->SetSkillDataKey(TEXT("AutoAttackNode_0"));
    Auto_1->SetSkillDataKey(TEXT("AutoAttackNode_1"));
    Auto_2->SetSkillDataKey(TEXT("AutoAttackNode_2"));

    EAttackWidget->SetSkillDataKey(TEXT("EAttackLevel"));
    E_0->SetSkillDataKey(TEXT("EAttackNode_0"));
    E_1->SetSkillDataKey(TEXT("EAttackNode_1"));
    E_2->SetSkillDataKey(TEXT("EAttackNode_2"));

    RAttackWidget->SetSkillDataKey(TEXT("RAttackLevel"));
    R_0->SetSkillDataKey(TEXT("RAttackNode_0"));
    R_1->SetSkillDataKey(TEXT("RAttackNode_1"));
    R_2->SetSkillDataKey(TEXT("RAttackNode_2"));

    PAttackWidget->SetSkillDataKey(TEXT("PAttackLevel"));
    P_0->SetSkillDataKey(TEXT("PAttackNode_0"));
    P_1->SetSkillDataKey(TEXT("PAttackNode_1"));
    P_2->SetSkillDataKey(TEXT("PAttackNode_2"));

}

void USkillWidget::UpdateCharacterData()
{
	Super::UpdateCharacterData();

}

void USkillWidget::SkillToMainStat(FSkillInfo Data)
{

}

void USkillWidget::SkillLevelUp()
{
    if (!LastRecoardSkillButtonWidget) return;

    int CurLevel = LastRecoardSkillButtonWidget->GetSkillLevel();

    UE_LOG(LogTemp, Error, TEXT("(LastRecoardSkillButtonWidget->GetMaxSkillLevel() %d , CurLevel : %d"), LastRecoardSkillButtonWidget->GetMaxSkillLevel(), CurLevel);

    if (LastRecoardSkillButtonWidget->GetMaxSkillLevel() < CurLevel + 1) return;

    LastRecoardSkillButtonWidget->SettingSkillLevel(++CurLevel);

    FString Key = LastRecoardSkillButtonWidget->GetSkillDataKey();

    // --- Auto Attack ---
    if (Key == TEXT("AutoAttackLevel")) { skillInfo.AutoAttackLevel++; }
    else if (Key == TEXT("AutoAttackNode_0")) { skillInfo.AutoAttackNode_0 = true; }
    else if (Key == TEXT("AutoAttackNode_1")) { skillInfo.AutoAttackNode_1 = true; }
    else if (Key == TEXT("AutoAttackNode_2")) { skillInfo.AutoAttackNode_2 = true; }

    // --- E Attack ---
    else if (Key == TEXT("EAttackLevel")) { skillInfo.EAttackLevel++; }
    else if (Key == TEXT("EAttackNode_0")) { skillInfo.EAttackkNode_0 = true; } // 구조체 오타 반영 (EAttackkNode_0)
    else if (Key == TEXT("EAttackNode_1")) { skillInfo.EAttackNode_1 = true; }
    else if (Key == TEXT("EAttackNode_2")) { skillInfo.EAttackNode_2 = true; }

    // --- R Attack ---
    else if (Key == TEXT("RAttackLevel")) { skillInfo.RAttackLevel++; }
    else if (Key == TEXT("RAttackNode_0")) { skillInfo.RAttackNode_0 = true; }
    else if (Key == TEXT("RAttackNode_1")) { skillInfo.RAttackNode_1 = true; }
    else if (Key == TEXT("RAttackNode_2")) { skillInfo.RAttackNode_2 = true; }

    // --- P Attack ---
    else if (Key == TEXT("PAttackLevel")) { skillInfo.PAttackLevel++; }
    else if (Key == TEXT("PAttackNode_0")) { skillInfo.PAttackNode_0 = true; }
    else if (Key == TEXT("PAttackNode_1")) { skillInfo.PAttackNode_1 = true; }
    else if (Key == TEXT("PAttackNode_2")) { skillInfo.PAttackNode_2 = true; }

    CharacterStat->SendSkillUpgradeToServer();
    
}

void USkillWidget::RecoardSkillButtonWidget(USkillButtonWidget* value)
{
    LastRecoardSkillButtonWidget = value;
}

void USkillWidget::UpdateWithServerData(const FSkillInfo& Data)
{

    skillInfo = Data;

    if (AutoAttackWidget) AutoAttackWidget->SettingSkillLevel(Data.AutoAttackLevel);
    if (EAttackWidget)    EAttackWidget->SettingSkillLevel(Data.EAttackLevel);
    if (RAttackWidget)    RAttackWidget->SettingSkillLevel(Data.RAttackLevel);
    if (PAttackWidget)    PAttackWidget->SettingSkillLevel(Data.PAttackLevel);

    Auto_0->SettingNodeImage(Data.AutoAttackNode_0);
    Auto_1->SettingNodeImage(Data.AutoAttackNode_1);
    Auto_2->SettingNodeImage(Data.AutoAttackNode_2);

    E_0->SettingNodeImage(Data.EAttackkNode_0);
    E_1->SettingNodeImage(Data.EAttackNode_1);
    E_2->SettingNodeImage(Data.EAttackNode_2);

    P_0->SettingNodeImage(Data.PAttackNode_0);
    P_1->SettingNodeImage(Data.PAttackNode_1);
    P_2->SettingNodeImage(Data.PAttackNode_2);

    R_0->SettingNodeImage(Data.RAttackNode_0);
    R_1->SettingNodeImage(Data.RAttackNode_1);
    R_2->SettingNodeImage(Data.RAttackNode_2);
    
}

FSkillInfo USkillWidget::GetSkillInfo()
{
    return skillInfo;
}
