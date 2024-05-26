// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/Material.h"
#include "Materials/MaterialInterface.h"
#include "Shield.generated.h"

UCLASS()
class SIDESCROLLER3D_API AShield : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShield();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		UStaticMeshComponent* SphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		UMaterialInterface* BlueMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		UMaterialInterface* HiddenMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	bool IsActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	int HitCounter = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	int MaxHitCounter = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float HitCounterRefreshTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float HitCounterMaxRefreshTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	bool IsFriendly = true;
	//UMaterialInterface* Material;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = SideScroller3D)
	void Hide();

	UFUNCTION(BlueprintCallable, Category = SideScroller3D)
	void Show();

	UFUNCTION(BlueprintCallable, Category = SideScroller3D)
	void RefreshCounterTick(float DeltaTime);

	int GetHitCounter() { 
		return HitCounter;
	};
	
	int GetMaxHitCounter() {
		return MaxHitCounter;
	};
};

