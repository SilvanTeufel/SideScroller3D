// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GAS.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbilityBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Ability")
	EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;

	
};
