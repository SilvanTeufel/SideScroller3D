// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Widgets/UnitBaseActionBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>

void UUnitBaseActionBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!OwnerUnitBase.IsValid() || !MagazineBar || !ShieldBar || !ManaBar)
		return;
	
	
	MagazineBar->SetPercent(float(OwnerUnitBase->MagSize[OwnerUnitBase->ActualWeaponId]) / float(OwnerUnitBase->MaxMagSize[OwnerUnitBase->ActualWeaponId]));
	
	ShieldBar->SetPercent( float(OwnerShield->GetHitCounter()) /float(OwnerShield->GetMaxHitCounter()) );

	ManaBar->SetPercent( OwnerUnitBase->Mana / OwnerUnitBase->MaxMana );
	
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	
	AvailableMagLabel->SetText(FText::AsNumber(OwnerUnitBase->MagSize[OwnerUnitBase->ActualWeaponId], &Opts));
	MaxMagLabel->SetText(FText::AsNumber(OwnerUnitBase->MaxMagSize[OwnerUnitBase->ActualWeaponId], &Opts));

	
	AvailableShieldLabel->SetText(FText::AsNumber(OwnerShield->GetHitCounter(), &Opts));
	MaxShieldLabel->SetText(FText::AsNumber(OwnerShield->GetMaxHitCounter(), &Opts));

	
	AvailableManaLabel->SetText(FText::AsNumber(OwnerUnitBase->Mana, &Opts));
	MaxManaLabel->SetText(FText::AsNumber(OwnerUnitBase->MaxMana, &Opts));
}