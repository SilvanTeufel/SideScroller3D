// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Unit/UnitBase.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Core/UnitData.h"
#include "MouseBotBase.generated.h"

UCLASS()
class SIDESCROLLER3D_API AMouseBotBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMouseBotBase();

	UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
	class UCapsuleComponent* TriggerCapsule;
	
	// declare overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void KillBot();

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "setAnimStateMouseBot", Keywords = "SideScroller3D setAnimStateEnemy"), Category = SideScroller3D)
	void SetAnimState(TEnumAsByte<BotData::BotStatus> NewCharAnimState);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "getAnimStateMouseBot", Keywords = "SideScroller3D getAnimStateEnemy"), Category = SideScroller3D)
	TEnumAsByte<BotData::BotStatus> GetAnimState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CharAnimStateMouseBot", Keywords = "SideScroller3D CharAnimStateEnemy"), Category = SideScroller3D)
	TEnumAsByte<BotData::BotStatus> CharAnimState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MoveToLocation", Keywords = "SideScroller3D CharAnimStateEnemy"), Category = SideScroller3D)
	FVector MoveToLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CurrentEnemy", Keywords = "SideScroller3D CurrentEnemy"), Category = SideScroller3D)
	AUnitBase* CurrentEnemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackBottom", Keywords = "SideScroller3D AttackBottom"), Category = SideScroller3D)
	bool AttackBottom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackIterationTime", Keywords = "SideScroller3D AttackIterationTime"), Category = SideScroller3D)
	float AttackIterationTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "AttackDamage", Keywords = "SideScroller3D AttackDamage"), Category = SideScroller3D)
	float AttackDamage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "DeathTime", Keywords = "SideScroller3D DeathTime"), Category = SideScroller3D)
	float DeathTime = 0.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "CurrentFriend", Keywords = "SideScroller3D CurrentFriend"), Category = SideScroller3D)
	AUnitBase* CurrentFriend;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealBottom", Keywords = "SideScroller3D HealBottom"), Category = SideScroller3D)
	bool HealBottom = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealIterationTime", Keywords = "SideScroller3D HealIterationTime"), Category = SideScroller3D)
	float HealIterationTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "HealPoints", Keywords = "SideScroller3D HealPoints"), Category = SideScroller3D)
	float HealPoints = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "LifeTime", Keywords = "SideScroller3D LifeTime"), Category = SideScroller3D)
	float LifeTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "MaxLifeTime", Keywords = "SideScroller3D MaxLifeTime"), Category = SideScroller3D)
	float MaxLifeTime = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	int TeamId = 1;
};
