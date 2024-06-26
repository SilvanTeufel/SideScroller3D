// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CameraBase.h" // Include the header file for ACameraBase
#include <Components/Button.h>
#include "Controller/CameraControllerBase.h"
#include "ExtendedCameraBase.generated.h"

/**
 * ExtendedCameraBase is a child class of ACameraBase
 */
UCLASS()
class SIDESCROLLER3D_API AExtendedCameraBase : public ACameraBase
{
	GENERATED_BODY()

public:

	AExtendedCameraBase(const FObjectInitializer& ObjectInitializer);
	
	// Override the BeginPlay function
	virtual void BeginPlay() override;
	
	// Override the Tick function
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SwitchControllerStateMachine(const FInputActionValue& InputActionValue, int32 NewCameraState);

	/** Handles Enhanced Keyboard Inputs */
	void Input_LeftClick_Pressed(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_LeftClick_Released(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_RightClick_Pressed(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_G_Pressed(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_A_Pressed(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_Ctrl_Pressed(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_Ctrl_Released(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_Tab_Pressed(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_Tab_Released(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_Shift_Pressed(const FInputActionValue& InputActionValue, int32 CamState);
	void Input_Shift_Released(const FInputActionValue& InputActionValue, int32 CamState);
	/** Handles Enhanced Keyboard Inputs */

	bool IsOwnedByLocalPlayer();
	// Abilitys + Talents /////
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = RTSUnitTemplate)
	class UWidgetComponent* TalentChooser;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = RTSUnitTemplate)
	class UWidgetComponent* AbilityChooser;
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
		void SetExtendedUserWidget(AUnitBase* SelectedActor);
	// Abilitys + Talents /////
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = RTSUnitTemplate)
	bool LevelWidgetToggled = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = RTSUnitTemplate)
	class UWidgetComponent* WidgetSelector;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,  Category = RTSUnitTemplate)
	class UWidgetComponent* ResourceWidget;
	
	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void OnAbilityInputDetected(EGASAbilityInputID InputID, AGASUnit* SelectedUnit, const TArray<TSubclassOf<UGameplayAbilityBase>>& AbilitiesArray);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	bool AutoAdjustTalentChooserPosition = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float JoyStickY = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float JoyStickX = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float JoyStick2Y = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	float JoyStick2X = 0.f;

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void FindUButtonWithMouseHover();

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void OnSimulatedMouseClick();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
	UButton* FocusedButton;
	
	void JoyStickMoveCamDependent(ACameraControllerBase* CameraControllerBase, float Value, int Angles[4]);
};