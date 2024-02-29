// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Hud/HUDBase.h"
#include "Characters/Camera/CameraBase.h"
#include "GameFramework/PlayerController.h"
#include "ControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API AControllerBase : public APlayerController
{
	GENERATED_BODY()

public:
	AControllerBase();

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	AHUDBase* HUDBase;
	ACameraBase* CameraBase;

public:

	void Tick(float DeltaSeconds);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShiftPressed", Keywords = "SideScroller3D ShiftPressed"), Category = SideScroller3D)
		void ShiftPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ShiftReleased", Keywords = "SideScroller3D ShiftReleased"), Category = SideScroller3D)
		void ShiftReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LeftClickPressed", Keywords = "SideScroller3D LeftClickPressed"), Category = SideScroller3D)
		void LeftClickPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LeftClickReleased", Keywords = "SideScroller3D LeftClickReleased"), Category = SideScroller3D)
		void LeftClickReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RightClickPressed", Keywords = "SideScroller3D RightClickPressed"), Category = SideScroller3D)
		void RightClickPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "APressed", Keywords = "SideScroller3D APressed"), Category = SideScroller3D)
		void APressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AReleased", Keywords = "SideScroller3D AReleased"), Category = SideScroller3D)
		void AReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AReleased", Keywords = "SideScroller3D AReleased"), Category = SideScroller3D)
		void SReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RPressed", Keywords = "SideScroller3D RPressed"), Category = SideScroller3D)
		void RPressed();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "FPressed", Keywords = "SideScroller3D FPressed"), Category = SideScroller3D)
		void FPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CPressed", Keywords = "SideScroller3D CPressed"), Category = SideScroller3D)
		void CPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EPressed", Keywords = "SideScroller3D EPressed"), Category = SideScroller3D)
		void EPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "XPressed", Keywords = "SideScroller3D XPressed"), Category = SideScroller3D)
		void XPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "QPressed", Keywords = "SideScroller3D QPressed"), Category = SideScroller3D)
		void QPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "QReleased", Keywords = "SideScroller3D QReleased"), Category = SideScroller3D)
		void QReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "TripleJump", Keywords = "SideScroller3D TripleJump"), Category = SideScroller3D)
		void TripleJump(FVector JumpDirection);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetDropJumpMine", Keywords = "SideScroller3D SetDropJumpMine"), Category = SideScroller3D)
		void SetDropJumpMine(bool DropMine);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "JumpCamera", Keywords = "SideScroller3D JumpCamera"), Category = SideScroller3D)
		void JumpCamera();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StrgPressed", Keywords = "SideScroller3D StrgPressed"), Category = SideScroller3D)
		void StrgPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "StrgReleased", Keywords = "SideScroller3D StrgReleased"), Category = SideScroller3D)
		void StrgReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomIn", Keywords = "SideScroller3D ZoomIn"), Category = SideScroller3D)
		void ZoomIn();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ZoomOut", Keywords = "SideScroller3D ZoomOut"), Category = SideScroller3D)
		void ZoomOut();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AltPressed", Keywords = "SideScroller3D AltPressed"), Category = SideScroller3D)
		void AltPressed();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AltReleased", Keywords = "SideScroller3D AltReleased"), Category = SideScroller3D)
		void AltReleased();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetAxis", Keywords = "SideScroller3D SetAxis"), Category = SideScroller3D)
	void SetAxis();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "FixAxis", Keywords = "SideScroller3D FixAxis"), Category = SideScroller3D)
	void FixAxis(bool IsX);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AltReleased", Keywords = "SideScroller3D AltReleased"), Category = SideScroller3D)
		void JoystickRunUnit(FVector2D AddDirection);
	
protected:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MouseBotStateMachine", Keywords = "SideScroller3D MouseBotStateMachine"), Category = SideScroller3D)
		void MouseBotStateMachine(float DeltaSeconds);

public:
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "IsShiftPressed", Keywords = "SideScroller3D IsShiftPressed"), Category = SideScroller3D)
		bool IsShiftPressed = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "AttackToggled", Keywords = "SideScroller3D AttackToggled"), Category = SideScroller3D)
		bool AttackToggled = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "IsStrgPressed", Keywords = "SideScroller3D IsStrgPressed"), Category = SideScroller3D)
		bool IsStrgPressed = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "IsSpacePressed", Keywords = "SideScroller3D IsSpacePressed"), Category = SideScroller3D)
		bool IsSpacePressed = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "AltIsPressed", Keywords = "SideScroller3D AltIsPressed"), Category = SideScroller3D)
		bool AltIsPressed = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "LeftClickisPressed", Keywords = "SideScroller3D LeftClickisPressed"), Category = SideScroller3D)
		bool LeftClickIsPressed = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "LockCameraToUnit", Keywords = "SideScroller3D LockCameraToUnit"), Category = SideScroller3D)
		bool LockCameraToUnit = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "AIsPressed", Keywords = "SideScroller3D AIsPressed"), Category = TopDownRTSCamLib)
		bool AIsPressed = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "DIsPressed", Keywords = "SideScroller3D DIsPressed"), Category = TopDownRTSCamLib)
		bool DIsPressed = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "WIsPressed", Keywords = "SideScroller3D WIsPressed"), Category = TopDownRTSCamLib)
		bool WIsPressed = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SIsPressed", Keywords = "SideScroller3D SIsPressed"), Category = TopDownRTSCamLib)
		bool SIsPressed = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "QIsPressed", Keywords = "SideScroller3D QIsPressed"), Category = TopDownRTSCamLib)
		bool QIsPressed = false;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSCamLib)
		bool YIsPressed = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedUnits", Keywords = "SideScroller3D SelectedUnits"), Category = SideScroller3D)
		TArray <AUnitBase*> SelectedUnits;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = RTSUnitTemplate)
		int SelectableTeamId = 0;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "BotIsToggled", Keywords = "SideScroller3D BotIsToggled"), Category = SideScroller3D)
		bool BotIsToggled = false;

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = RTSUnitTemplate)
		void SetControlerTeamId(int Id);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = RTSUnitTemplate)
		void MoveToLocationUEPathFinding(AUnitBase* Unit, const FVector& DestinationLocation);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = RTSUnitTemplate)
		void SetRunLocation(AUnitBase* Unit, const FVector& DestinationLocation);

	UPROPERTY(Replicated, BlueprintReadWrite, Category = RTSUnitTemplate)
		float UEPathfindingCornerOffset = 100.f;

	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSCamLib)
		bool RShoulder2Pressed = false;
	
	UPROPERTY(BlueprintReadWrite, Category = TopDownRTSCamLib)
		bool LShoulder2Pressed = false;
};
