// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScatterBall.generated.h"

UCLASS()
class SIDESCROLLER3D_API AScatterBall : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScatterBall();
	
	void Init(float BallDamage, float BallScale, bool XSpeedIsPos, bool YSpeedIsPos, UMaterialInterface* Material, bool ZSpeedIsPos);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "TriggerCapsule", Keywords = "SideScroller3D TriggerCapsule"), Category = SideScroller3D)
	class UCapsuleComponent* TriggerCapsule;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, Category = SideScroller3D)
	float Damage;

	UPROPERTY(BlueprintReadWrite, Category = SideScroller3D)
	float LifeTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float MaxLifeTime = 2.f;

	UPROPERTY(BlueprintReadWrite, Category = SideScroller3D)
	int TeamId = 0;

	UPROPERTY(BlueprintReadWrite, Category = SideScroller3D)
	float MovementSpeed = 50.f;

	float SpeedX;
	float SpeedY;
	float SpeedZ;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
