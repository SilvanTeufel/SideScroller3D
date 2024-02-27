// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Controller/Input/InputConfig.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Components/WidgetComponent.h"
#include "Actors/UIWeaponIndicator.h"
#include "Core/UnitData.h"
#include "InputMappingContext.h"
#include "CameraBase.generated.h"


UCLASS()
class SIDESCROLLER3D_API ACameraBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	//ACameraBase(const FObjectInitializer& ObjectInitializer);

	UCapsuleComponent* GetCameraBaseCapsule() const {
		return GetCapsuleComponent();
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputConfig* InputConfig;

	//void SwitchControllerStateMachine(const FInputActionValue& InputActionValue, int32 NewCameraState);


	bool BlockControls = true;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateCameraComp", Keywords = "SideScroller3D CreateCameraComp"), Category = SideScroller3D)
		void CreateCameraComp();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "RootScene", Keywords = "SideScroller3D RootScene"), Category = SideScroller3D)
		USceneComponent* RootScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpringArm", Keywords = "SideScroller3D SpringArm"), Category = SideScroller3D)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "SpringArmRotator", Keywords = "SideScroller3D SpringArmRotator"), Category = SideScroller3D)
		FRotator SpringArmRotator = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CameraComp", Keywords = "SideScroller3D CameraComp"), Category = SideScroller3D)
		UCameraComponent* CameraComp;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "PC", Keywords = "SideScroller3D PC"), Category = SideScroller3D)
		APlayerController* PC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MappingContext", Keywords = "SideScroller3D MappingContext"), Category = TopDownRTSCamLib)
		UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MappingPriority", Keywords = "SideScroller3D MappingPriority"), Category = TopDownRTSCamLib)
		int32 MappingPriority;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CameraDistanceToCharacter", Keywords = "SideScroller3D CameraDistanceToCharacter"), Category = TopDownRTSCamLib)
		float CameraDistanceToCharacter;

	UFUNCTION( BlueprintCallable, meta = (DisplayName = "PanMoveCamera", Keywords = "SideScroller3D PanMoveCamera"), Category = SideScroller3D)
		void PanMoveCamera(const FVector& NewPanDirection);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Margin", Keywords = "SideScroller3D Margin"), Category = SideScroller3D)
		float Margin = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScreenSizeX", Keywords = "SideScroller3D ScreenSizeX"), Category = SideScroller3D)
		int32 ScreenSizeX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScreenSizeY", Keywords = "SideScroller3D ScreenSizeY"), Category = SideScroller3D)
		int32 ScreenSizeY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GetViewPortScreenSizesState", Keywords = "SideScroller3D GetViewPortScreenSizesState"), Category = SideScroller3D)
		int GetViewPortScreenSizesState = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CamSpeed", Keywords = "SideScroller3D CamSpeed"), Category = SideScroller3D)
		float CamSpeed = 80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CamSpeed", Keywords = "SideScroller3D CamSpeed"), Category = SideScroller3D)
		float ZoomSpeed = 300;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomIn", Keywords = "SideScroller3D ZoomIn"), Category = SideScroller3D)
		void ZoomIn();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomOut", Keywords = "SideScroller3D ZoomOut"), Category = SideScroller3D)
		void ZoomOut();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamLeft", Keywords = "SideScroller3D RotateCamLeft"), Category = TopDownRTSCamLib)
		bool RotateCamLeft(bool UseCamRotationOffset);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamRight", Keywords = "SideScroller3D RotateCamRight"), Category = TopDownRTSCamLib)
		bool RotateCamRight(bool UseCamRotationOffset);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamLeft", Keywords = "SideScroller3D RotateCamLeft"), Category = TopDownRTSCamLib)
		bool RotateCamLeftTo(float Position);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RotateCamRight", Keywords = "SideScroller3D RotateCamRight"), Category = TopDownRTSCamLib)
		bool RotateCamRightTo(float Position);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "JumpCamera", Keywords = "SideScroller3D JumpCamera"), Category = SideScroller3D)
		void JumpCamera(FHitResult Hit);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMousePos2D", Keywords = "SideScroller3D GetMousePos2D"), Category = SideScroller3D)
		FVector2D GetMousePos2D();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Zoom", Keywords = "SideScroller3D Zoom"), Category = SideScroller3D)
		void Zoom();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomOutToPosition", Keywords = "SideScroller3D ZoomOutToPosition"), Category = RTSUnitTemplate)
		void ZoomOutToPosition(float Distance, const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomInToPosition", Keywords = "SideScroller3D ZoomInToPosition"), Category = RTSUnitTemplate)
		bool ZoomInToPosition(float Distance, const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockOnUnit", Keywords = "SideScroller3D LockOnUnit"), Category = SideScroller3D)
		void LockOnUnit(AUnitBase* Unit);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZoomOutPosition", Keywords = "SideScroller3D ZoomOutPosition"), Category = SideScroller3D)
		float ZoomOutPosition = 4000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZoomPosition", Keywords = "SideScroller3D ZoomPosition"), Category = SideScroller3D)
		float ZoomPosition = 1700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "PitchValue", Keywords = "SideScroller3D PitchValue"), Category = SideScroller3D)
		float PitchValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "YawValue", Keywords = "SideScroller3D YawValue"), Category = SideScroller3D)
		float YawValue = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "RollValue", Keywords = "SideScroller3D RollValue"), Category = SideScroller3D)
		float RollValue = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZoomXYDistance", Keywords = "SideScroller3D ZoomXYDistance"), Category = SideScroller3D)
		float ZoomXYDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CamZOffset", Keywords = "SideScroller3D CamZOffset"), Category = SideScroller3D)
		float CamZOffset = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CamZOffset", Keywords = "SideScroller3D CamZOffset"), Category = SideScroller3D)
		float LastActorZeroPosition = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DeltaZScaler", Keywords = "SideScroller3D DeltaZScaler"), Category = SideScroller3D)
		float DeltaZScaler = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DeltaDecrementer", Keywords = "SideScroller3D DeltaDecrementer"), Category = SideScroller3D)
		float DeltaDecrementer = 1.f;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomThirdPersonPosition", Keywords = "SideScroller3D ZoomThirdPersonPosition"), Category = TopDownRTSCamLib)
		bool IsCharacterDistanceTooLow(float Distance, const FVector SelectedActorPosition = FVector(0.f,0.f,0.f));
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZoomThirdPersonPosition", Keywords = "SideScroller3D ZoomThirdPersonPosition"), Category = TopDownRTSCamLib)
		float ZoomThirdPersonPosition = 600.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CamRotationOffset", Keywords = "SideScroller3D CamRotationOffset"), Category = TopDownRTSCamLib)
		float CamRotationOffset = 11.5f;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "CameraAngles", Keywords = "SideScroller3D CameraAngles"), Category = TopDownRTSCamLib)
		float CameraAngles[4] = { 0.f, 90.f, 180.f, 270.f };

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "RotationDegreeStep", Keywords = "SideScroller3D RotationDegreeStep"), Category = TopDownRTSCamLib)
		float RotationDegreeStep = 90.f;

	
	bool IsCameraInAngle()
	{
		if(YawValue == 360.f) YawValue = 0.f;
		bool IsInAngle = false;
		for( int i = 0; i < 4 ; i++)
		{
			if(YawValue == CameraAngles[i]) IsInAngle = true;
		}
		return IsInAngle;
	}

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToForward", Keywords = "SideScroller3D MoveCamToForward"), Category = TopDownRTSCamLib)
		void MoveCamToForward();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToBackward", Keywords = "SideScroller3D MoveCamToBackward"), Category = TopDownRTSCamLib)
		void MoveCamToBackward();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToLeft", Keywords = "SideScroller3D MoveCamToLeft"), Category = TopDownRTSCamLib)
		void MoveCamToLeft();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveCamToRight", Keywords = "SideScroller3D MoveCamToRight"), Category = TopDownRTSCamLib)
		void MoveCamToRight();
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "StartTime", Keywords = "SideScroller3D StartTime"), Category = TopDownRTSCamLib)
		float StartTime = 0.f;
		
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SwitchAxis", Keywords = "SideScroller3D SwitchAxis"), Category = TopDownRTSCamLib)
		bool SwitchAxis = false;
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetComp", Keywords = "SideScroller3D ControlWidgetComp"), Category = SideScroller3D)
		class UWidgetComponent* ControlWidgetComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetRotator", Keywords = "SideScroller3D ControlWidgetRotator"), Category = SideScroller3D)
		FRotator ControlWidgetRotation = FRotator(50, 180, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetLocation", Keywords = "SideScroller3D ControlWidgetLocation"), Category = SideScroller3D)
		FVector ControlWidgetLocation = FVector(400.f, -100.0f, -0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ControlWidgetHideLocation", Keywords = "SideScroller3D ControlWidgetHideLocation"), Category = SideScroller3D)
		FVector ControlWidgetHideLocation = FVector(400.f, -4500.0f, -0.0f);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "HideControlWidget", Keywords = "SideScroller3D HideControlWidget"), Category = SideScroller3D)
		void HideControlWidget();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShowControlWidget", Keywords = "SideScroller3D ShowControlWidget"), Category = SideScroller3D)
		void ShowControlWidget();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnControlWidget", Keywords = "SideScroller3D SpawnControlWidget"), Category = SideScroller3D)
		void SpawnControlWidget();

	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "ActionWidgetComp", Keywords = "SideScroller3D ActionWidgetComp"), Category = SideScroller3D)
		class UWidgetComponent* ActionWidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (DisplayName = "UIWeaponIndicator", Keywords = "SideScroller3D UIWeaponIndicator"), Category = SideScroller3D)
		class AUIWeaponIndicator* UIWeaponIndicator;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "WeaponIndictatorLocation", Keywords = "SideScroller3D WeaponIndictatorLocation"), Category = SideScroller3D)
		FVector WeaponIndictatorLocation = FVector(50, 0, -50);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ActionWidgetRotation", Keywords = "SideScroller3D ActionWidgetRotation"), Category = SideScroller3D)
		FRotator ActionWidgetRotation = FRotator(0.f, 0.f, 0.f);
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ActionWidgetLocation", Keywords = "SideScroller3D ActionWidgetLocation"), Category = SideScroller3D)
    	FVector ActionWidgetLocation = FVector(0.f, 0.f, -150.f);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetUserWidget", Keywords = "SideScroller3D SetUserWidget"), Category = SideScroller3D)
		 void SetUserWidget(AExtendedUnitBase* SelectedActor);
    
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeWeaponIndicator", Keywords = "SideScroller3D ChangeWeaponIndicator"), Category = SideScroller3D)
    	 void ChangeWeaponIndicator(class USkeletalMesh* NewWeaponMesh);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnWeaponIndicator", Keywords = "SideScroller3D SpawnWeaponIndicator"), Category = SideScroller3D)
		void SpawnWeaponIndicator();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnActionWidget", Keywords = "SideScroller3D SpawnActionWidget"), Category = SideScroller3D)
		void SpawnActionWidget();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CameraState", Keywords = "SideScroller3D CameraState"), Category = SideScroller3D)
		TEnumAsByte<CameraData::CameraState> CameraState = CameraData::ZoomInPosition;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetCameraState", Keywords = "SideScroller3D SetCameraState"), Category = SideScroller3D)
		void SetCameraState(TEnumAsByte<CameraData::CameraState> NewCameraState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCameraState", Keywords = "SideScroller3D GetCameraState"), Category = SideScroller3D)
		TEnumAsByte<CameraData::CameraState> GetCameraState();
};
