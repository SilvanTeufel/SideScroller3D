// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Actors/ScatterBall.h"
#include "ScatterMine.generated.h"

UCLASS()
class SIDESCROLLER3D_API AScatterMine : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AScatterMine();

	void Init(AActor* TargetActor, AActor* ShootingActor, FVector LocationToSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Target", Keywords = "SideScroller3D Target"), Category = SideScroller3D)
	AActor* Target;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TriggerCapsule", Keywords = "SideScroller3D TriggerCapsule"), Category = SideScroller3D)
	class UCapsuleComponent* TriggerCapsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	FVector TargetLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float Damage = 40.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float LifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float MaxLifeTime = 30.f;

	UPROPERTY(BlueprintReadWrite, Category = SideScroller3D)
	float MovementSpeed = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	int TeamId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	bool SpawnComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float ScaleScatterBall = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	int ScatterBallCount= 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	FVector SpawnOffset = FVector(0.f,0.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ScatterBallMaterial", Keywords = "ScatterBallMaterial SideScroller3D"), Category = SideScroller3D)
	UMaterialInterface* ScatterBallMaterial;
	
	TArray<AScatterBall*> ScatterBalls;
	
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SpawnScatterBalls();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "ZSpeedIsPos", Keywords = "ZSpeedIsPos SideScroller3D"), Category = SideScroller3D)
	bool ZSpeedIsPos = true;
};
