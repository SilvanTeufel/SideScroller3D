// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/UIWeaponIndicator.h"

// Sets default values
AUIWeaponIndicator::AUIWeaponIndicator()
{
    WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
    if (WeaponMesh) {
        WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // QueryAndPhysics
        WeaponMesh->SetupAttachment(RootComponent);
 
        if(WeaponMesh)
        {
            WeaponMesh->SetRelativeLocation(WeaponIndicatorPosition);
            FQuat QuadRotation = FQuat(WeaponIndicatorRotation);
            WeaponMesh->SetRelativeRotation(QuadRotation);
            WeaponMesh->SetCastShadow(false);
        }
    }
}

// Called when the game starts or when spawned
void AUIWeaponIndicator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUIWeaponIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUIWeaponIndicator::ChangeWeaponIndicator(USkeletalMesh* NewWeaponMesh)
{
    if (WeaponMesh) {
        WeaponMesh->SetSkeletalMesh(NewWeaponMesh);
        WeaponMesh->SetCastShadow(false);
    }
}

