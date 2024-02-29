// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/MouseBotBase.h"
#include "Actors/ScatterMine.h"
#include "Actors/Shield.h"
#include "Actors/Glider.h"
#include "ExtendedUnitBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API AExtendedUnitBase : public AUnitBase
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	// Weaponbase related //////////////
protected:
	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "WeaponClass", Keywords = "SideScroller3D WeaponClass"), Category = SideScroller3D)
	TSubclassOf<class AWeaponBase> WeaponClass;
	
public:
	UPROPERTY(Visibleanywhere, BlueprintReadWrite, meta = (DisplayName = "Weapon", Keywords = "SideScroller3D Weapon"), Category = SideScroller3D)
	class AWeaponBase* Weapon;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponSpawn", Keywords = "SideScroller3D WeaponSpawn"), Category = SideScroller3D)
		void WeaponSpawn();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeWeapon", Keywords = "SideScroller3D ChangeWeapon"), Category = SideScroller3D)
		void ChangeWeapon(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Fire", Keywords = "SideScroller3D Fire"), Category = SideScroller3D)
		void Fire();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponMesh", Keywords = "SideScroller3D GetWeaponMesh"), Category = SideScroller3D)
		class USkeletalMesh* GetWeaponMesh();

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "WeaponArray", Keywords = "SideScroller3D WeaponArray"), Category = SideScroller3D)
	TArray<FString> Weapons;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MaxMagSize", Keywords = "SideScroller3D MaxMagSize"), Category = SideScroller3D)
	TArray<int> MaxMagSize;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MagSize", Keywords = "SideScroller3D MagSize"), Category = SideScroller3D)
	TArray<int> MagSize;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetAmmo", Keywords = "SideScroller3D SetAmmo"), Category = SideScroller3D)
	void SetAmmo(int NewAmmo);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetAmmo", Keywords = "SideScroller3D GetAmmo"), Category = SideScroller3D)
	float GetAmmo();
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ReloadWeapon", Keywords = "SideScroller3D ReloadWeapon"), Category = SideScroller3D)
		void ReloadWeapon();

	UFUNCTION(BlueprintCallable,  Category = SideScroller3D)
		bool TabNextUnitToChase();
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ActualWeaponId", Keywords = "SideScroller3D ActualWeaponId"), Category = SideScroller3D)
		int ActualWeaponId = 1;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ProjectileMaterial", Keywords = "SideScroller3D ProjectileMaterial"), Category = SideScroller3D)
		UMaterialInterface* ProjectileMaterial;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ProjectileMesh", Keywords = "SideScroller3D ProjectileMesh"), Category = SideScroller3D)
		UStaticMesh* ProjectileMesh;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ReloadTime", Keywords = "SideScroller3D ReloadTime"), Category = SideScroller3D)
	   float ReloadTime;
	   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackDuration", Keywords = "SideScroller3D AttackDuration"), Category = SideScroller3D)
       	float AttackDuration = 0.6f;
	//////////////////////////////////////


	// Teleport related /////////
	UPROPERTY(Editanywhere, meta = (DisplayName = "TeleportStartTime", Keywords = "SideScroller3D TeleportStartTime"), Category = SideScroller3D)
	float TeleportStartTime = 1.f;

	UPROPERTY(Editanywhere, meta = (DisplayName = "TeleportEndTime", Keywords = "SideScroller3D TeleportEndTime"), Category = SideScroller3D)
	float TeleportEndTime = 0.5f;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "TeleportRadius", Keywords = "SideScroller3D TeleportRadius"), Category = SideScroller3D)
	float TeleportRadius = 70.f;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "TeleportFailedDmg", Keywords = "SideScroller3D TeleportFailedDmg"), Category = SideScroller3D)
	float TeleportFailedDmg = 10.f; // When TeleportStartTime reaches TeleportStopEndTime States is changes to Idle

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "TeleportLocation", Keywords = "SideScroller3D TeleportLocation"), Category = SideScroller3D)
	FVector TeleportLocation;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "TeleportManaCost", Keywords = "SideScroller3D TeleportManaCost"), Category = SideScroller3D)
	float TeleportManaCost = 30.f;
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateTeleportRadius", Keywords = "SideScroller3D CreateTeleportRadius"), Category = SideScroller3D)
	void CreateTeleportRadius();
	//////////////////////////////////////


	// ScatterMine related /////////
	public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScatterMineBaseClass", Keywords = "SideScroller3D ScatterMineBaseClass"), Category = SideScroller3D)
	TSubclassOf<class AScatterMine> ScatterMineBaseClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ScatterMine", Keywords = "SideScroller3D ScatterMine"), Category = SideScroller3D)
	class AScatterMine*  ScatterMine;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnScatterMine", Keywords = "SideScroller3D SpawnScatterMine"), Category = SideScroller3D)
	void SpawnScatterMine(AActor* Target, AActor* Attacker, bool CostMana, TSubclassOf<class AScatterMine> ScatterMineClass);

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ScatterMineSpawnLocation", Keywords = "SideScroller3D ScatterMineSpawnLocation"), Category = SideScroller3D)
	FVector ScatterMineSpawnLocation;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScatterMineCastTime", Keywords = "SideScroller3D ScatterMineCastTime"), Category = SideScroller3D)
	float ScatterMineCastTime = 0.4f;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateMineSpawnRadius", Keywords = "SideScroller3D CreateMineSpawnRadius"), Category = SideScroller3D)
	void CreateMineSpawnRadius();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MineSpawnRadius", Keywords = "SideScroller3D MineSpawnRadius"), Category = SideScroller3D)
	float MineSpawnRadius = 8.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MineManaCost", Keywords = "SideScroller3D MineManaCost"), Category = SideScroller3D)
	float MineManaCost = 15.f;
	//////////////////////////////////////


	
	// SelectedIcon related /////////
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIconActionRadius", Keywords = "SideScroller3D CreateIconActionRadius"), Category = SideScroller3D)
	void CreateIconActionRadius();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIconNormalRadius", Keywords = "SideScroller3D CreateIconNormalRadius"), Category = SideScroller3D)
	void CreateIconNormalRadius();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateIconJumpRadius", Keywords = "SideScroller3D CreateIconJumpRadius"), Category = SideScroller3D)
	void CreateIconJumpRadius(FVector Color);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateColoredIconNormalRadius", Keywords = "SideScroller3D CreateColoredIconNormalRadius"), Category = SideScroller3D)
	void CreateColoredIconNormalRadius(FVector Color);
	//////////////////////////////////////

	
	/// Spellshield related /////////
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ShieldBaseClass", Keywords = "SideScroller3D ShieldBaseClass"), Category = SideScroller3D)
	TSubclassOf<class AShield> ShieldBaseClass;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Shield", Keywords = "SideScroller3D Shield"), Category = SideScroller3D)
	class AShield* Shield;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ToggleShield", Keywords = "SideScroller3D ToggleShield"), Category = SideScroller3D)
	void ToggleShield();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnShield", Keywords = "SideScroller3D SpawnShield"), Category = SideScroller3D)
	void SpawnShield();
	//////////////////////////////////////

	
	/// Mousebot related /////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MouseBotBaseClass", Keywords = "SideScroller3D MouseBotBaseClass"), Category = SideScroller3D)
	TSubclassOf<class AMouseBotBase> MouseBotBaseClass;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MouseBotSpawnLocation", Keywords = "SideScroller3D MouseBotSpawnLocation"), Category = SideScroller3D)
	FVector MouseBotSpawnLocation;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "MouseBot", Keywords = "SideScroller3D MouseBot"), Category = SideScroller3D)
	class AMouseBotBase* MouseBot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MouseBotCastTime", Keywords = "SideScroller3D MouseBotCastTime"), Category = SideScroller3D)
	float MouseBotCastTime = 0.4f;

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateMineSpawnRadius", Keywords = "SideScroller3D CreateMineSpawnRadius"), Category = SideScroller3D)
	void CreateMouseBotSpawnRadius();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MouseBotRadius", Keywords = "SideScroller3D MouseBotRadius"), Category = SideScroller3D)
	float MouseBotRadius = 15.f;

	UPROPERTY(Editanywhere, BlueprintReadWrite, meta = (DisplayName = "BotManaCost", Keywords = "SideScroller3D BotManaCost"), Category = SideScroller3D)
	float BotManaCost = 5.f;
    	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnMouseBot", Keywords = "SideScroller3D SpawnMouseBot"), Category = SideScroller3D)
	void SpawnMouseBot();
	///////////////////////////////////////

	
	/// Mana related /////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxMana", Keywords = "SideScroller3D MaxMana"), Category = SideScroller3D)
	float MaxMana = 100.f;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Mana", Keywords = "SideScroller3D Mana"), Category = SideScroller3D)
	float Mana = MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ManaRestore", Keywords = "SideScroller3D ManaRestore"), Category = SideScroller3D)
	float ManaRestore = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ManaRestoreTime", Keywords = "SideScroller3D ManaRestoreTime"), Category = SideScroller3D)
	float ManaRestoreTime = 1.f;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "ManaTime", Keywords = "SideScroller3D ManaTime"), Category = SideScroller3D)
	float ManaTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "NoManaTime", Keywords = "SideScroller3D NoManaTime"), Category = SideScroller3D)
	float NoManaTime = 1.5f;
	
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "RestoreMana", Keywords = "SideScroller3D RestoreMana"), Category = SideScroller3D)
	void RestoreMana(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SetMana", Keywords = "SideScroller3D SetMana"), Category = SideScroller3D)
	void SetMana(float NewMana);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetMana", Keywords = "SideScroller3D GetMana"), Category = SideScroller3D)
	float GetMana();
	///////////////////////////////////////


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpTime", Keywords = "SideScroller3D JumpTime"), Category = SideScroller3D)
	float JumpTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpPauseTime", Keywords = "SideScroller3D JumpPauseTime"), Category = SideScroller3D)
	float JumpPauseTime = 0.1f;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "JumpLocation", Keywords = "SideScroller3D JumpLocation"), Category = SideScroller3D)
	FVector JumpLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpLoad", Keywords = "SideScroller3D JumpLoad"), Category = SideScroller3D)
	float JumpLoad = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpLoad", Keywords = "SideScroller3D JumpLoad"), Category = SideScroller3D)
	float JumpLoadIncrement = 2000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxJumpLoad", Keywords = "SideScroller3D MaxJumpLoad"), Category = SideScroller3D)
	float MaxJumpLoad = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpIconScaleDivider", Keywords = "SideScroller3D JumpIconScaleDivider"), Category = SideScroller3D)
	float JumpIconScaleDivider = 550.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpLoadOffset", Keywords = "SideScroller3D JumpLoadOffset"), Category = SideScroller3D)
	float JumpLoadOffset = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpCounter", Keywords = "SideScroller3D JumpCounter"), Category = SideScroller3D)
	int JumpCounter = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpCounterResetTime", Keywords = "SideScroller3D JumpCounterResetTime"), Category = SideScroller3D)
	float JumpCounterResetTime = 6.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpCounterResetTime", Keywords = "SideScroller3D JumpCounterResetTime"), Category = SideScroller3D)
	float JumpCounterTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxJumps", Keywords = "SideScroller3D MaxJumps"), Category = SideScroller3D)
	int MaxJumps = 3.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpMineClass", Keywords = "SideScroller3D JumpMineClass"), Category = SideScroller3D)
	TSubclassOf<class AScatterMine> JumpMineClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "JumpMineSpawnDelta", Keywords = "SideScroller3D JumpMineSpawnDelta"), Category = SideScroller3D)
	float JumpMineSpawnDelta = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DropJumpMine", Keywords = "SideScroller3D DropJumpMine"), Category = SideScroller3D)
	bool DropJumpMine = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GliderBaseClass", Keywords = "SideScroller3D GliderBaseClass"), Category = SideScroller3D)
	TSubclassOf<class AGlider> GliderBaseClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Glider", Keywords = "SideScroller3D Glider"), Category = SideScroller3D)
	class AGlider* Glider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	FName GliderSocketName = FName("ik_foot_rSocket");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	FVector GliderSpawnOffset = FVector(0, 0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	FRotator GliderSpawnRotator = FRotator(90.f, 0.f, 0.f);
	
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "SpawnGlider", Keywords = "SideScroller3D SpawnGlider"), Category = SideScroller3D)
	void SpawnGlider();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "DeSpawnGlider", Keywords = "SideScroller3D DeSpawnGlider"), Category = SideScroller3D)
	void DeSpawnGlider();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "GliderSpawned", Keywords = "SideScroller3D GliderSpawned"), Category = SideScroller3D)
	bool GliderSpawned = false;

	UPROPERTY(BlueprintReadWrite, Category = SideScroller3D)
	int DespawnGliderState = 0;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "XAxisFixedPosition", Keywords = "SideScroller3D XAxisFixedPosition"), Category = TopDownRTSCamLib)
	float XAxisFixedPosition;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "YAxisFixedPosition", Keywords = "SideScroller3D YAxisFixedPosition"), Category = TopDownRTSCamLib)
	float YAxisFixedPosition;
};

