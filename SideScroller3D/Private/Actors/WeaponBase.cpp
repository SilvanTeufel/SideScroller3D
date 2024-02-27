// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Characters/Unit/UnitBase.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComp;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	TArray<FString> WeaponNames = GetWeaponNames();

	if (WeaponDataTable)
	{
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponNames[1]), WeaponNames[1], true); // AR-15 // AS-VAL
		if (WeaponData) {
			MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
		}
	}
}

void AWeaponBase::ChangeWeapon(FString WeaponName) {
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		if (WeaponData) {
			MeshComp->SetSkeletalMesh(WeaponData->WeaponMesh);
		}
	}
}

float AWeaponBase::GetWeaponRange(FString WeaponName)
{

	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Range;
	}
	return 0.f;
}

int AWeaponBase::GetWeaponMagSize(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->MagSize;
	}
	return 0;
}

float AWeaponBase::GetWeaponDamage(FString WeaponName)
{

	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Damage;
	}
	return 0.f;
}

float AWeaponBase::GetWeaponTime(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Time;
	}
	return 0.f;
}

FString AWeaponBase::GetWeaponType(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->Type;
	}
	return "Unknown";
}

USkeletalMesh* AWeaponBase::GetWeaponMesh(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->WeaponMesh;
	}
	return nullptr;
}

TArray<FString> AWeaponBase::GetWeaponNames()
{
	if (WeaponDataTable)
	{
		TArray<FName> Names = WeaponDataTable->GetRowNames();
		TArray<FString> SNames;
		for (int Index = 0; Index != Names.Num(); ++Index)
		{
			SNames.Emplace(Names[Index].ToString());
			
		}
		return TArray<FString>(SNames);
	}
	else {
		return TArray<FString>();
	}
}

UStaticMesh* AWeaponBase::GetProjectileMesh(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileMesh;
	}
	return nullptr;
}

UMaterialInterface* AWeaponBase::GetProjectileMaterial(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileMaterial;
	}
	return nullptr;
}

FVector AWeaponBase::GetProjectileScale(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileScale;
	}
	return FVector(1.f,1.f,1.f);
}

float AWeaponBase::GetReloadTime(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ReloadTime;
	}
	return 1.f;
}

float AWeaponBase::GetProjectileSpeed(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->ProjectileSpeed;
	}
	return 1.f;
}

float AWeaponBase::GetPauseDuration(FString WeaponName)
{
	if (WeaponDataTable)
	{
		static const FString PString = FString(WeaponName); // "AR-15DT"
		WeaponData = WeaponDataTable->FindRow<FWeaponData>(FName(WeaponName), PString, true); // AR-15 // AS-VAL
		return WeaponData->PauseDuration;
	}
	return 1.f;
}

void AWeaponBase::PlayFireAnimation()
{
	if (WeaponData && WeaponData->FireAnimation) {
		MeshComp->PlayAnimation(WeaponData->FireAnimation, false);
	}
}

void AWeaponBase::PlayReloadAnimation()
{
	if (WeaponData && WeaponData->ReloadAnimation) {
		MeshComp->PlayAnimation(WeaponData->ReloadAnimation, false);
	}
}

void AWeaponBase::WeaponOverlap(class AActor* overlappedActor, class AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		AUnitBase* UnitBase = Cast<AUnitBase>(otherActor);
		//UnitBase->WeaponSpawn();
	}
}
