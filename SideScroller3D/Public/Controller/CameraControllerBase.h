// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Controller/ControllerBase.h"
#include "Controller/Input/EnhancedInputComponentBase.h"
#include "CameraControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API ACameraControllerBase : public AControllerBase
{
	GENERATED_BODY()
	
	ACameraControllerBase();

public:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetViewPortScreenSizes", Keywords = "SideScroller3D GetViewPortScreenSizes"), Category = TopDownRTSCamLib)
	void GetViewPortScreenSizes(int x);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetCameraPanDirection", Keywords = "SideScroller3D GetCameraPanDirection"), Category = TopDownRTSCamLib)
	FVector GetCameraPanDirection();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CameraBaseMachine", Keywords = "SideScroller3D CameraBaseMachine"), Category = TopDownRTSCamLib)
	void CameraBaseMachine(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ToggleLockCamToCharacter", Keywords = "SideScroller3D ToggleLockCamToCharacter"), Category = TopDownRTSCamLib)
	void ToggleLockCamToCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "UnlockCamFromCharacter", Keywords = "SideScroller3D UnlockCamFromCharacter"), Category = TopDownRTSCamLib)
	void UnlockCamFromCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToCharacter", Keywords = "SideScroller3D LockCamToCharacter"), Category = TopDownRTSCamLib)
	void LockCamToCharacter();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "LockCamToCharacter", Keywords = "SideScroller3D LockCamToCharacter"), Category = TopDownRTSCamLib)
	void LockZDistanceToCharacter();
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "LockCameraToCharacter", Keywords = "SideScroller3D LockCameraToCharacter"), Category = TopDownRTSCamLib)
	bool LockCameraToCharacter = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsRotatingRight", Keywords = "SideScroller3D CamIsRotatingRight"), Category = TopDownRTSCamLib)
	bool CamIsRotatingRight = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsRotatingLeft", Keywords = "SideScroller3D CamIsRotatingLeft"), Category = TopDownRTSCamLib)
	bool CamIsRotatingLeft = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsZoomingIn", Keywords = "SideScroller3D CamIsZoomingIn"), Category = TopDownRTSCamLib)
	bool CamIsZoomingIn = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CamIsZoomingOut", Keywords = "SideScroller3D CamIsZoomingOut"), Category = TopDownRTSCamLib)
	bool CamIsZoomingOut= false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ZoomOutToPosition", Keywords = "SideScroller3D ZoomOutToPosition"), Category = TopDownRTSCamLib)
	bool ZoomOutToPosition = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ZoomInToPosition", Keywords = "SideScroller3D ZoomInToPosition"), Category = TopDownRTSCamLib)
	bool ZoomInToPosition = true;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "HoldZoomOnLockedCharacter", Keywords = "SideScroller3D HoldZoomOnLockedCharacter"), Category = TopDownRTSCamLib)
	bool HoldZoomOnLockedCharacter = false;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "LockAlwaysToCharacter", Keywords = "SideScroller3D LockAlwaysToCharacter"), Category = TopDownRTSCamLib)
	bool LockAlwaysToCharacter = true;
};
