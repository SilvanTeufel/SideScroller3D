// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Actors/Shield.h"
#include "UnitBaseActionBar.generated.h"

/**
 * 
 */
UCLASS()
class SIDESCROLLER3D_API UUnitBaseActionBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetOwnerActor(AExtendedUnitBase* Unit) {
		OwnerUnitBase = Unit;
		
		if(Unit->Shield)
			OwnerShield = Unit->Shield;
	}


protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY()
		TWeakObjectPtr<AExtendedUnitBase> OwnerUnitBase;

	UPROPERTY()
		TWeakObjectPtr<AShield> OwnerShield;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* MagazineBar;
	
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ShieldBar;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ManaBar;
	
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableMagLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxMagLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableShieldLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxShieldLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AvailableManaLabel;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxManaLabel;
};
