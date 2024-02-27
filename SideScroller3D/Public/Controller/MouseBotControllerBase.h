// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/MouseBotBase.h"
#include "MouseBotControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API AMouseBotControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AMouseBotControllerBase();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MouseBotStateMachine", Keywords = "SideScroller3D MouseBotStateMachine"), Category = SideScroller3D)
	void MouseBotStateMachine(float DeltaTime);
};
