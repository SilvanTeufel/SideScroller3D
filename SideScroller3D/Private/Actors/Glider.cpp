// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/Glider.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AGlider::AGlider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = MeshComp;
}

// Called when the game starts or when spawned
void AGlider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

