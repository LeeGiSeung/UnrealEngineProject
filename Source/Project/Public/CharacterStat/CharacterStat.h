// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ECharacterMenuState/ECharacterMenuState.h"
#include "FCharacterStatAnimaionDataTable/FCharacterStatAnimaionDataTable.h"
#include "Serverinfo/FCharacterDataWrapper/FCharacterDataWrapper.h"

//Server
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"

#include "CharacterStat.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UStatAnimInstance;

class USkillWidget;
class URelicWidget;
class UStarWidget;
class UMainWidget;

UCLASS()
class PROJECT_API ACharacterStat : public ACharacter
{
	GENERATED_BODY()

//Server
public:
	UPROPERTY()
	FCharacterDataWrapper CharacterDataWrapper;

	void SetBeginServerData();
	void OnCharacterDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY()
	USkillWidget* SkillWidget;

	UPROPERTY()
	URelicWidget* RelicWidget;

	UPROPERTY()
	UStarWidget* StarWidget;

	UPROPERTY()
	UMainWidget* MainWidget;

	UMainWidget* GetMainWidget();

	void FindWidgetClass(USkillWidget* Skill, UMainWidget* Main, URelicWidget* Relic, UStarWidget* Star);

	UPROPERTY()
	FMaininfo MainStat;

	void SetMainStat(FMaininfo value);
	FMaininfo GetMainStat();

public:
	// Sets default values for this character's properties
	ACharacterStat();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "SwitchCamera")
	void SwitchCameraComponent(ECharacterMenuState MenuState);

	// 카메라를 지탱해줄 팔 (길이, 회전 제어용)
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	class USpringArmComponent* CameraBoom;

	// 실제 화면을 보여줄 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* ViewCamera;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void SetStatFollowCamera(UCameraComponent* value);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* RelicCamera;  

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* SkillCamera;  

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* StarCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* MainCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Camera")
	ECharacterMenuState TestState;

	FVector TargetLocation;
	FRotator TargetRotation;

	// 보간 속도 (숫자가 높을수록 빠릅니다. 스타레일 느낌은 5.0 ~ 8.0 추천)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float CameraInterpSpeed = 5.0f;

	UCameraComponent* TargetCam = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void ChangeCamera();

	void RestoreCamera();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<FName, FCharacterStatAnimaionDataTable> CharacterAnimMap;

	// 현재 어떤 캐릭터인지 구분할 변수
	UPROPERTY(EditAnywhere, Category = "Animation")
	FName CurrentCharacterKey = TEXT("BASE");

	UFUNCTION()
	void SetCurrentCharacterKey(FName value);

	UFUNCTION()
	FName GetCurrentCharacterKey();

public:
	UFUNCTION()
	void PlayAnimation(ECharacterMenuState value);

	UPROPERTY()
	UStatAnimInstance* StatAnimInstance;

	ECharacterMenuState CurMenuState;

};
