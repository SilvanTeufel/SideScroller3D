// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Actors/Component.h"
#include "Actors/Waypoint.h"
#include "Characters/Unit/UnitBase.h"
#include "Engine/World.h"
#include "ComponentGeneratorGameModeBase.generated.h"

UCLASS()
class SIDESCROLLER3D_API AComponentGeneratorGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AComponentGeneratorGameModeBase();
	void EndGame();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	FVector CalcLocation(FVector Offset, FVector MinRange, FVector MaxRange);
	FRotator FRandomRotator(FRotator Min, FRotator Max);
	FRotator GetRotationRange(FString Name);
	FRotator GetAbsoluteRotationOffset(FString Name);
	FVector  GetComponentScale(FString Name);
	class UStaticMesh* GetComponentMesh(FString Name);
	FVector  GetComponentRandRangeMin(FString Name);
	FVector  GetComponentRandRangeMax(FString Name);
	FVector  GetComponentAbsoluteOffset(FString Name);
	bool  GetComponentIsMoving(FString Name);
	FVector  GetComponentMovementSpeed(FString Name);
	float  GetComponentMovementRange(FString Name);
	UMaterial* GetComponentMaterial(FString Name);
	int  GetComponentFrequency(FString Name);
	ESpawnActorCollisionHandlingMethod GetCollisionMethod(FString Name);
	TArray<FString> GetComponentNames();
	TArray<FString> ComponentNames;

	class AComponentController* ComponentController;

protected:

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "ComponentClass", Keywords = "SideScroller3D ComponentClass"), Category = SideScroller3D)
	TSubclassOf<class AComponent> ComponentClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	class UDataTable* ComponentDataTable;
	
	FComponentData* ComponentData;
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = SideScroller3D)
	TArray<class AComponent*> Components;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ComponentSpawn", Keywords = "SideScroller3D ComponentSpawn"), Category = SideScroller3D)
	void ComponentSpawn();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnEnemys", Keywords = "SideScroller3D SpawnEnemys"), Category = SideScroller3D)
	void SpawnEnemys();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DespawnDeadEnemys", Keywords = "SideScroller3D DespawnDeadEnemys"), Category = SideScroller3D)
	void DespawnDeadEnemys();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RespawnAllEnemys", Keywords = "SideScroller3D RespawnAllEnemys"), Category = SideScroller3D)
	void RespawnAllEnemys();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateSecondWaypointLocation", Keywords = "SideScroller3D CreateSecondWaypointLocation"), Category = SideScroller3D)
	FVector CreateSecondWaypointLocation(FVector FirstLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = CheckSpawnTimer)
	float CheckSpawnTimer = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "WaypointClass", Keywords = "SideScroller3D WaypointClass"), Category = SideScroller3D)
	TSubclassOf<class AWaypoint> WaypointClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = SideScroller3D)
	TArray<class AWaypoint*> FirstWaypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = SideScroller3D)
	TArray<class AWaypoint*> SecondWaypoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitBaseClass", Keywords = "SideScroller3D UnitBaseClass"), Category = SideScroller3D)
	TSubclassOf<class AUnitBase> UnitBaseClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintReadWrite, Category = SideScroller3D)
	TArray<class AUnitBase*> UnitBases;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitCount", Keywords = "SideScroller3D UnitCount"), Category = SideScroller3D)
	int UnitCount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitCountToRespawn", Keywords = "SideScroller3D UnitCountToRespawn"), Category = SideScroller3D)
	int UnitCountToRespawn = 10 ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitOffset", Keywords = "SideScroller3D UnitOffset"), Category = SideScroller3D)
	FVector UnitOffset = FVector(3000.f,3000.f,1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitMinRange", Keywords = "SideScroller3D UnitMinRange"), Category = SideScroller3D)
	FVector UnitMinRange = FVector(0.f,0.f,0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "UnitMaxRange", Keywords = "SideScroller3D UnitMaxRange"), Category = SideScroller3D)
	FVector UnitMaxRange = FVector(2000.f,2000.f,0.f);



	
};
