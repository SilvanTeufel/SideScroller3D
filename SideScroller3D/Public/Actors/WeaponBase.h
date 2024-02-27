// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "Actors/Projectile.h"
#include "WeaponBase.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		class USkeletalMesh* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		FString WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		class UAnimationAsset* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		class UAnimationAsset* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		int MagSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		float Time;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		FString Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		UStaticMesh* ProjectileMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		UMaterialInterface* ProjectileMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		FVector ProjectileScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		float ProjectileSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		float PauseDuration;
};

UCLASS()
class SIDESCROLLER3D_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "WeaponOverlap", Keywords = "SideScroller3D WeaponOverlap"), Category = SideScroller3D)
	void WeaponOverlap(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "ChangeWeapon", Keywords = "SideScroller3D ChangeWeapon"), Category = SideScroller3D)
	void ChangeWeapon(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponRange", Keywords = "SideScroller3D GetWeaponRange"), Category = SideScroller3D)
	float GetWeaponRange(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponMagSize", Keywords = "SideScroller3D GetWeaponMagSize"), Category = SideScroller3D)
	int GetWeaponMagSize(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponDamage", Keywords = "SideScroller3D GetWeaponDamage"), Category = SideScroller3D)
	float GetWeaponDamage(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponTime", Keywords = "SideScroller3D GetWeaponTime"), Category = SideScroller3D)
	float GetWeaponTime(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponType", Keywords = "SideScroller3D GetWeaponType"), Category = SideScroller3D)
	FString GetWeaponType(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponMesh", Keywords = "SideScroller3D GetWeaponMesh"), Category = SideScroller3D)
	class USkeletalMesh* GetWeaponMesh(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetWeaponNames", Keywords = "SideScroller3D GetWeaponNames"), Category = SideScroller3D)
	TArray<FString> GetWeaponNames();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileMesh", Keywords = "SideScroller3D GetProjectileMesh"), Category = SideScroller3D)
	UStaticMesh* GetProjectileMesh(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileMaterial", Keywords = "SideScroller3D GetProjectileMaterial"), Category = SideScroller3D)
	UMaterialInterface* GetProjectileMaterial(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileSize", Keywords = "SideScroller3D GetProjectileSize"), Category = SideScroller3D)
	FVector GetProjectileScale(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetReloadTime", Keywords = "SideScroller3D GetReloadTime"), Category = SideScroller3D)
	float GetReloadTime(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetProjectileSpeed", Keywords = "SideScroller3D GetProjectileSpeed"), Category = SideScroller3D)
	float GetProjectileSpeed(FString WeaponName);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PauseDuration", Keywords = "SideScroller3D PauseDuration"), Category = SideScroller3D)
	float GetPauseDuration(FString WeaponName);
	
protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		class UDataTable* WeaponDataTable;

	FWeaponData* WeaponData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PlayFireAnimation", Keywords = "SideScroller3D PlayFireAnimation"), Category = SideScroller3D)
	void PlayFireAnimation();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "PlayReloadAnimation", Keywords = "SideScroller3D PlayReloadAnimation"), Category = SideScroller3D)
	void PlayReloadAnimation();
};
