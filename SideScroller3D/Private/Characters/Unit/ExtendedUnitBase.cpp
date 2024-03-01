// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Characters/Unit/ExtendedUnitBase.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/SelectedIcon.h"
#include "Actors/WeaponBase.h"


void AExtendedUnitBase::BeginPlay()
{
	Super::BeginPlay();

	ToggleUnitDetection = false;
	WeaponSpawn();
	SpawnShield();
	
	for (int i = 0; i < Weapons.Num(); i++) {
		MaxMagSize.Emplace(Weapon->GetWeaponMagSize(Weapons[i]));
	}
	
	MagSize = MaxMagSize;
}

void AExtendedUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RestoreMana(DeltaTime);
	//TeleportCoolDownTick(DeltaTime);
}

// Weaponbase related //////////////////////////////////////////////////
void AExtendedUnitBase::WeaponSpawn()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (WeaponClass) {

		FTransform WeaponTransform;
		WeaponTransform.SetLocation(FVector::ZeroVector);
		WeaponTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		Weapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, WeaponTransform, SpawnParams);

		if (Weapon) {
			Weapons = Weapon->GetWeaponNames();
			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("hand_rSocket"));
			
			// GETTING PROJECTILE INFORMATION FROM THE DATATABLE //
			ProjectileMaterial = Weapon->GetProjectileMaterial(Weapons[ActualWeaponId]);
			ProjectileMesh = Weapon->GetProjectileMesh(Weapons[ActualWeaponId]);
			ProjectileScale =  Weapon->GetProjectileScale(Weapons[ActualWeaponId]);
			Attributes->SetAttackDamage(Weapon->GetWeaponDamage(Weapons[ActualWeaponId]));
			
			AttackDuration = Weapon->GetWeaponTime(Weapons[ActualWeaponId]);
			Attributes->SetRange(Weapon->GetWeaponRange(Weapons[ActualWeaponId]));
	
			ReloadTime = Weapon->GetReloadTime(Weapons[ActualWeaponId]);
			Attributes->SetProjectileSpeed(Weapon->GetProjectileSpeed(Weapons[ActualWeaponId]));
		
			//PauseDuration = Weapon->GetPauseDuration(Weapons[ActualWeaponId]);
		}
	}
}

void AExtendedUnitBase::ChangeWeapon(FString WeaponName) {
	if (WeaponClass) {
		if (Weapon) {
			// To Handsocket
			Weapon->ChangeWeapon(WeaponName);

			// GETTING PROJECTILE INFORMATION FROM THE DATATABLE //
			ProjectileMaterial = Weapon->GetProjectileMaterial(Weapons[ActualWeaponId]);
			ProjectileMesh = Weapon->GetProjectileMesh(Weapons[ActualWeaponId]);
			ProjectileScale =  Weapon->GetProjectileScale(Weapons[ActualWeaponId]);
			Attributes->SetAttackDamage(Weapon->GetWeaponDamage(Weapons[ActualWeaponId]));
			AttackDuration = Weapon->GetWeaponTime(Weapons[ActualWeaponId]);
			Attributes->SetRange(Weapon->GetWeaponRange(Weapons[ActualWeaponId]));
			ReloadTime = Weapon->GetReloadTime(Weapons[ActualWeaponId]);
			Attributes->SetProjectileSpeed(Weapon->GetProjectileSpeed(Weapons[ActualWeaponId]));
		
			//PauseDuration = Weapon->GetPauseDuration(Weapons[ActualWeaponId]);
		}
	}
}


void AExtendedUnitBase::SpawnWeaponProjectile(AActor* Target, AActor* Attacker) // FVector TargetLocation
{
	AUnitBase* ShootingUnit = Cast<AUnitBase>(Attacker);

	if(ShootingUnit)
	{
		FTransform Transform;
		Transform.SetLocation(GetActorLocation() + Attributes->GetProjectileScaleActorDirectionOffset()*GetActorForwardVector() + ProjectileSpawnOffset);

		FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FRotator InitialRotation = Direction.Rotation() + ProjectileRotationOffset;

		Transform.SetRotation(FQuat(InitialRotation));
		Transform.SetScale3D(ShootingUnit->ProjectileScale);
		
		const auto MyProjectile = Cast<AProjectile>
							(UGameplayStatics::BeginDeferredActorSpawnFromClass
							(this, ProjectileBaseClass, Transform,  ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (MyProjectile != nullptr)
		{
			MyProjectile->Init(Target, Attacker);
			if (MyProjectile->Mesh)
			{
				MyProjectile->Mesh->SetStaticMesh(ProjectileMesh);
				MyProjectile->Mesh->SetMaterial(0, ProjectileMaterial);
			}

			MyProjectile->Mesh->OnComponentBeginOverlap.AddDynamic(MyProjectile, &AProjectile::OnOverlapBegin);


			
		
			UGameplayStatics::FinishSpawningActor(MyProjectile, Transform);
		}
	}
}

void AExtendedUnitBase::Fire()
{
	Weapon->PlayFireAnimation();
}

void AExtendedUnitBase::ReloadWeapon() {
	if (WeaponClass) {
		if (Weapon) {
			// To Handsocket
			MagSize[ActualWeaponId] = MaxMagSize[ActualWeaponId];
			SetUnitState(UnitData::Reload);
			Weapon->PlayReloadAnimation();
		}
	}
}

USkeletalMesh* AExtendedUnitBase::GetWeaponMesh()
{
	if(!Weapon)
		return nullptr;

	return Weapon->GetWeaponMesh(Weapons[ActualWeaponId]);

}


void AExtendedUnitBase::CreateMineSpawnRadius()
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < MineSpawnRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 1.2);
		}
		else if (IconScale.X - 0.5 > MineSpawnRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 0.95);
		}
		SelectedIcon->ChangeMaterialToAction();
	}
}

void AExtendedUnitBase::SpawnScatterMine(AActor* Target, AActor* Attacker, bool CostMana, TSubclassOf<class AScatterMine> ScatterMineClass)
{
	AExtendedUnitBase* ShootingUnit = Cast<AExtendedUnitBase>(Attacker);
	
	if(ShootingUnit)
	{
		FTransform Transform;
		Transform.SetRotation(FQuat(FRotator::ZeroRotator)); 
		
		const auto MyScatterMine = Cast<AScatterMine>
							(UGameplayStatics::BeginDeferredActorSpawnFromClass
							(this, ScatterMineClass, Transform,  ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
		if (MyScatterMine != nullptr)
		{
			MyScatterMine->Init(Target, Attacker, ScatterMineSpawnLocation);
			UGameplayStatics::FinishSpawningActor(MyScatterMine, Transform);
			if(CostMana)
			Mana -= MineManaCost;
		}
	}
}

void AExtendedUnitBase::CreateTeleportRadius()
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < TeleportRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 1.2);
		}
		else if (IconScale.X - 0.5 > TeleportRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 0.95);
		}
		SelectedIcon->ChangeMaterialToAction();
	}
}

void AExtendedUnitBase::CreateIconActionRadius()
{
	if (Weapon) {

		if (SelectedIcon) {
			FVector IconScale = SelectedIcon->GetActorScale3D();
			IconScale.X = Attributes->GetRange()/35+0.5;
			IconScale.Y = Attributes->GetRange()/35+0.5;
			SelectedIcon->SetActorScale3D(IconScale);
			SelectedIcon->ChangeMaterialToAction();

		}
	}
}

void AExtendedUnitBase::CreateIconNormalRadius()
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < 1.0f) {
			SelectedIcon->SetActorScale3D(FVector(1.0f));
			
		}
		else if (IconScale.X - 0.5 > 1.0f) {
			SelectedIcon->SetActorScale3D(FVector(1.0f));
		}

		SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 40.f, 30.f, 0.5f));
		
		if (Shield) {
			if (Shield->IsActive)
				SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 10.f, 100.f, 0.5f));
			else
				SelectedIcon->ChangeMaterialColour(FVector4d(5.f, 40.f, 30.f, 0.5f));
		}
	}
}


void AExtendedUnitBase::CreateColoredIconNormalRadius(FVector Color)
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < 1.0f) {
			SelectedIcon->SetActorScale3D(FVector(1.0f));
			
		}
		else if (IconScale.X - 0.5 > 1.0f) {
			SelectedIcon->SetActorScale3D(FVector(1.0f));
		}
		
		SelectedIcon->ChangeMaterialColour(FVector4d(Color.X, Color.Y, Color.Z, 0.5f));

		
	}
}

void AExtendedUnitBase::CreateIconJumpRadius(FVector Color)
{
	if (SelectedIcon) {

		FVector IconScale = SelectedIcon->GetActorScale3D();
		IconScale.X = JumpLoad/JumpIconScaleDivider;
		IconScale.Y = JumpLoad/JumpIconScaleDivider;
		SelectedIcon->SetActorScale3D(IconScale);
		
		SelectedIcon->ChangeMaterialColour(FVector4d(Color.X, Color.Y, Color.Z, 0.5f));
			
	
	}
}

void AExtendedUnitBase::ToggleShield()
{
	if (Shield) {
		if (Shield->IsActive) {
			Shield->IsActive = false;
			Shield->Hide();
			HealthWidgetComp->bHiddenInGame = false;
		}
		else if (Shield->HitCounter > 0) {
			Shield->IsActive = true;
			Shield->Show();
			HealthWidgetComp->bHiddenInGame = true;

		}
	}

}
void AExtendedUnitBase::SpawnShield()
{
	if(!ShieldBaseClass)
		return;
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpellTransform;
	SpellTransform.SetLocation(FVector(0, 0, 0));
	SpellTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	Shield = GetWorld()->SpawnActor<AShield>(ShieldBaseClass, SpellTransform, SpawnParams);

	if (Shield)
	{
		Shield->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("rootSocket"));
		Shield->SetActorScale3D(FVector(2.0f));
		Shield->SetActorRelativeLocation(FVector(0, 0, 80));
		Shield->IsActive = false;
		Shield->Hide();
	}
}

void AExtendedUnitBase::RestoreMana(float DeltaTime)
{
	ManaTime += DeltaTime;
	if(ManaTime >= ManaRestoreTime)
	{
		ManaTime = 0;
		Mana < MaxMana? Mana += ManaRestore : Mana = MaxMana;
	}
	
}

void AExtendedUnitBase::SpawnGlider()
{
	if(!GliderBaseClass || GliderSpawned)
		return;
		
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpellTransform;
	SpellTransform.SetLocation(FVector(0, 0, 0));
	SpellTransform.SetRotation(FQuat(FRotator(0.f, 0.f, 0.f)));

	Glider = GetWorld()->SpawnActor<AGlider>(GliderBaseClass, SpellTransform, SpawnParams);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
	
	if (Glider)
	{
		Glider->AttachToComponent(GetMesh(), AttachmentRules, GliderSocketName);
		Glider->SetActorRelativeLocation(GliderSpawnOffset);
		Glider->SetActorRelativeRotation(GliderSpawnRotator);
		GliderSpawned = true;
	}
}

void AExtendedUnitBase::DeSpawnGlider()
{
	if(!GliderSpawned)
		return;
	
	Glider->Destroy(true, false);
	GliderSpawned = false;
}

void AExtendedUnitBase::CreateMouseBotSpawnRadius()
{
	if (SelectedIcon) {
		FVector IconScale = SelectedIcon->GetActorScale3D();
		if (IconScale.X + 0.5 < MouseBotRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 1.2);
		}
		else if (IconScale.X - 0.5 > MouseBotRadius) {
			SelectedIcon->SetActorScale3D(IconScale * 0.95);
		}
		SelectedIcon->ChangeMaterialToAction();
	}
}

void AExtendedUnitBase::SpawnMouseBot()
{
	if(!MouseBotBaseClass)
		return;

	FTransform Transform;
	Transform.SetLocation(FVector(MouseBotSpawnLocation));
	
	const auto Bot = Cast<AMouseBotBase>
		(UGameplayStatics::BeginDeferredActorSpawnFromClass
		(this, MouseBotBaseClass, Transform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));
	
	UGameplayStatics::FinishSpawningActor(Bot, Transform);
	
	MouseBot = Bot;
	MouseBot->SetAnimState(BotData::Run_Bot);
	Mana -= BotManaCost;
}

void AExtendedUnitBase::SetMana(float NewMana)
{

	if(NewMana <= 0.f)
	{
		Mana = 0.f;
	}else if(NewMana >= MaxMana)
	{
		Mana = MaxMana;
	}else
	{
		Mana = NewMana;
	}
}



float AExtendedUnitBase::GetMana()
{
	return Mana;
}

void AExtendedUnitBase::SetAmmo(int NewAmmo)
{

	if(NewAmmo <= 0.f)
	{
		MagSize[ActualWeaponId] = 0.f;
	}else if(NewAmmo >= MaxMagSize[ActualWeaponId])
	{
		MagSize[ActualWeaponId] = MaxMagSize[ActualWeaponId];
	}else
	{
		MagSize[ActualWeaponId] = NewAmmo;
	}
}



float AExtendedUnitBase::GetAmmo()
{
	return MagSize[ActualWeaponId];
}

bool AExtendedUnitBase::TabNextUnitToChase()
{
	if(!UnitsToChase.Num()) return false;
		
	
	//int NewIndex = 0;
	bool RValue = false;

	UE_LOG(LogTemp, Warning, TEXT("UnitsToChase.Num(): %d"), UnitsToChase.Num());
	if(UnitToChaseIndex >= UnitsToChase.Num()-1)
		UnitToChaseIndex = 0;
	
	for(int i = UnitToChaseIndex; i < UnitsToChase.Num(); i++)
	{
		if(UnitsToChase[i] && UnitsToChase[i] != UnitToChase && UnitsToChase[i]->GetUnitState() != UnitData::Dead)
		{
				UE_LOG(LogTemp, Warning, TEXT("Tabbed to the Next Unit!"));
				UnitToChaseIndex = i;
				UnitToChase->SetDeselected();
				UnitToChase = UnitsToChase[UnitToChaseIndex];
				UnitToChase->SetSelected();
				RValue = true;
				break;
		}
	}
	
	TArray <AUnitBase*> UnitsToDelete = UnitsToChase;
	
	for(int i = 0; i < UnitsToDelete.Num(); i++)
	{
		if(UnitsToDelete[i] && UnitsToDelete[i]->GetUnitState() == UnitData::Dead)
		{
			UnitsToChase.Remove(UnitsToDelete[i]);
		}
	}

	return RValue;
}


bool AExtendedUnitBase::TabPrevUnitToChase()
{
	if(!UnitsToChase.Num()) return false;
		
	
	//int NewIndex = 0;
	bool RValue = false;

	UE_LOG(LogTemp, Warning, TEXT("UnitsToChase.Num(): %d"), UnitsToChase.Num());
	if(UnitToChaseIndex <= 0)
		UnitToChaseIndex = UnitsToChase.Num()-1;
	
	for(int i = UnitToChaseIndex; i >= 0; i--)
	{
		if(UnitsToChase[i] && UnitsToChase[i] != UnitToChase && UnitsToChase[i]->GetUnitState() != UnitData::Dead)
		{
			UE_LOG(LogTemp, Warning, TEXT("Tabbed to the Next Unit!"));
			UnitToChaseIndex = i;
			UnitToChase->SetDeselected();
			UnitToChase = UnitsToChase[UnitToChaseIndex];
			UnitToChase->SetSelected();
			RValue = true;
			break;
		}
	}
	
	TArray <AUnitBase*> UnitsToDelete = UnitsToChase;
	
	for(int i = 0; i < UnitsToDelete.Num(); i++)
	{
		if(UnitsToDelete[i] && UnitsToDelete[i]->GetUnitState() == UnitData::Dead)
		{
			UnitsToChase.Remove(UnitsToDelete[i]);
		}
	}

	return RValue;
}
