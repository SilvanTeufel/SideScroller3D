// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Core/UnitData.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Sound\SoundCue.h"
#include "UnitBaseAnimInstance.generated.h"


USTRUCT(BlueprintType)
struct FUnitAnimData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	TEnumAsByte<UnitData::EState> AnimState;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float BlendPoint_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float BlendPoint_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float TransitionRate_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float TransitionRate_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float Resolution_1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float Resolution_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	USoundBase* Sound;
};


/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class SIDESCROLLER3D_API UUnitBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UUnitBaseAnimInstance();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	AActor* OwningActor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		TEnumAsByte<UnitData::EState> CharAnimState;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float BlendPoint_1 = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float BlendPoint_2 = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float CurrentBlendPoint_1 = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float CurrentBlendPoint_2 = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float TransitionRate_1 = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float TransitionRate_2 = 0.5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float Resolution_1 = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float Resolution_2 = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float AttackTempoScale = 1.75;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	USoundBase* Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float SoundTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	float SoundMinDeltaTime = 0.4f;
	
	TEnumAsByte<UnitData::EState> LastAnimState = UnitData::None;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float Deltaseconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		class UDataTable* AnimDataTable;

	void SetAnimTableData(TEnumAsByte<UnitData::EState> AnimState);
	void SetBlendPoints(TEnumAsByte<UnitData::EState> AnimState);
	USoundBase* GetAnimSound(TEnumAsByte<UnitData::EState> AnimState);

	bool JumpLoadSoundPlayed = false;
	
	FUnitAnimData* UnitAnimData;
	
};