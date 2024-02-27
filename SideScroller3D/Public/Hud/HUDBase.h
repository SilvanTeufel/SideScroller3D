// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerController.h"
#include "Characters/Unit/UnitBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/MouseBotBase.h"
#include "HUDBase.generated.h"



/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API AHUDBase : public AHUD
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DrawHUD", Keywords = "SideScroller3D DrawHUD"), Category = SideScroller3D)
		virtual void DrawHUD(); // used in Tick();

	void Tick(float DeltaSeconds);

	void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "bSelectFriendly", Keywords = "SideScroller3D bSelectFriendly"), Category = SideScroller3D)
	bool bSelectFriendly = false;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "InitialPoint", Keywords = "SideScroller3D InitialPoint"), Category = SideScroller3D)
		FVector2D InitialPoint; // Position of mouse on screen when pressed;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CurrentPoint", Keywords = "SideScroller3D CurrentPoint"), Category = SideScroller3D)
		FVector2D CurrentPoint; // Position of mouse on screen while holding;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "IPoint", Keywords = "SideScroller3D IPoint"), Category = SideScroller3D)
		FVector IPoint = FVector(0.f,0.f, 0.f); // Position of mouse on screen when pressed;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "CPoint", Keywords = "SideScroller3D CPoint"), Category = SideScroller3D)
		FVector CPoint = FVector(0.f,0.f, 0.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "RectangleScaleSelectionFactor", Keywords = "SideScroller3D RectangleScaleSelectionFactor"), Category = SideScroller3D)
		float RectangleScaleSelectionFactor = 0.9;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMousePos2D", Keywords = "SideScroller3D GetMousePos2D"), Category = SideScroller3D)
		FVector2D GetMousePos2D();

	UFUNCTION(BlueprintCallable, Category = RTSUnitTemplate)
	void AssignNewHighestIndex(AUnitBase* Unit);
	
	UPROPERTY(BlueprintReadWrite, Category = RTSUnitTemplate)
		int32 HighestUnitIndex = 0;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAllUnits", Keywords = "SideScroller3D GetAllUnits"), Category = SideScroller3D)
		void GetAllUnits();
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedEnemyUnits", Keywords = "SideScroller3D SelectedEnemyUnits"), Category = SideScroller3D)
	    TArray <AUnitBase*> SelectedUnits;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "FriendlyUnits", Keywords = "RTSUnitTemplate FriendlyUnits"), Category = RTSUnitTemplate)
		TArray <AUnitBase*> FriendlyUnits;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "SelectedEnemyUnits", Keywords = "SideScroller3D SelectedEnemyUnits"), Category = SideScroller3D)
		TArray <AUnitBase*> AllUnits;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "EnemyUnitBases", Keywords = "RTSUnitTemplate EnemyUnitBases"), Category = RTSUnitTemplate)
		TArray <AUnitBase*> EnemyUnitBases;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "AllUnitBases", Keywords = "RTSUnitTemplate AllUnitBases"), Category = RTSUnitTemplate)
		TArray <AUnitBase*> AllUnitBases;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "FoundMouseBots", Keywords = "SideScroller3D FoundMouseBots"), Category = SideScroller3D)
		TArray <AMouseBotBase*> MouseBots;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RemoveMouseBot", Keywords = "SideScroller3D RemoveMouseBot"), Category = SideScroller3D)
		void RemoveMouseBot(AMouseBotBase* MouseBotBase);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CharacterIsUnSelectable", Keywords = "SideScroller3D CharacterIsUnSelectable"), Category = SideScroller3D)
		bool CharacterIsUnSelectable = false;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "MoveUnitsThroughWayPoints", Keywords = "SideScroller3D MoveUnitsThroughWayPoints"), Category = SideScroller3D)
		void MoveUnitsThroughWayPoints(TArray <AUnitBase*> Units);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetUnitSelected", Keywords = "SideScroller3D SetUnitSelected"), Category = SideScroller3D)
		void SetUnitSelected(AUnitBase* Unit);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DeselectAllUnits", Keywords = "SideScroller3D DeselectAllUnits"), Category = SideScroller3D)
		void DeselectAllUnits();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAllMouseBots", Keywords = "SideScroller3D GetAllMouseBots"), Category = SideScroller3D)
		void GetAllMouseBots();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ControllDirectionToMouse", Keywords = "SideScroller3D ControllDirectionToMouse"), Category = SideScroller3D)
		void ControllDirectionToMouse(AActor* Units, FHitResult Hit);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsActorInsideRectangle", Keywords = "SideScroller3D IsActorInsideRectangle"), Category = SideScroller3D)
		bool IsActorInsideRec(FVector InPoint, FVector CuPoint, FVector ALocation);
};
