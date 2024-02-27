// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Controller/UnitControllerBase.h"
#include "Hud/HUDBase.h"
#include "ExtendedUnitControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API AExtendedUnitControllerBase : public AUnitControllerBase
{
	GENERATED_BODY()
	
public:
	AExtendedUnitControllerBase();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateJumpColor", Keywords = "SideScroller3D CreateJumpColor"), Category = SideScroller3D)
	void CreateJumpColor(AExtendedUnitBase* ExtendedUnitBase);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ExtendedUnitControlMachine", Keywords = "SideScroller3D ExtendedUnitControlMachine"), Category = SideScroller3D)
	void ExtendedUnitControlMachine(float DeltaSeconds);
	
};
