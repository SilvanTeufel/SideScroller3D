// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Glider.generated.h"

UCLASS()
class SIDESCROLLER3D_API AGlider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGlider();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SideScroller3D) //BlueprintReadWrite, Category="Weapon-Specific"
	class USkeletalMeshComponent* MeshComp;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
