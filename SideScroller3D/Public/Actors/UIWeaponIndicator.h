// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "UIWeaponIndicator.generated.h"

UCLASS()
class SIDESCROLLER3D_API AUIWeaponIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIWeaponIndicator();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
		USkeletalMeshComponent* WeaponMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = SideScroller3D)
	void ChangeWeaponIndicator(class USkeletalMesh* NewWeaponMesh);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	FVector WeaponIndicatorPosition = FVector(-500.f, 400.0f, -290.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D)
	FRotator WeaponIndicatorRotation = FRotator(-45.f, 0.0f, 0.f);

};
