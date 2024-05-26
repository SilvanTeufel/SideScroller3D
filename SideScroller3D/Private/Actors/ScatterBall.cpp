// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Actors/ScatterBall.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Unit/UnitBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AScatterBall::AScatterBall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryAndPhysics
	Mesh->SetCollisionProfileName(TEXT("Trigger")); // Kollisionsprofil festlegen
	Mesh->SetGenerateOverlapEvents(true); // Überlappungsevents aktivieren

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("/Engine/BasicShapes/Sphere"));
	
	if (SphereMeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(SphereMeshAsset.Object);
	}
}

// Called when the game starts or when spawned
void AScatterBall::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScatterBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime+=DeltaTime;
	
	if(LifeTime >= MaxLifeTime){
		Destroy(true, false);
	}else
	{
		FVector ALoc = GetActorLocation();
		SetActorLocation(FVector(ALoc.X+SpeedX, ALoc.Y+SpeedY, ALoc.Z+SpeedZ));
	}
	
}

void AScatterBall::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor)
	{
		AUnitBase* UnitToHit = Cast<AUnitBase>(OtherActor);

		if(UnitToHit && UnitToHit->TeamId != TeamId)
		{
			UnitToHit->SetHealth(UnitToHit->Attributes->GetHealth()-Damage);
			UnitToHit->SetUnitState( UnitData::IsAttacked );
			if(UnitToHit->GetUnitState() != UnitData::IsAttacked )UnitToHit->UnitControlTimer = 0.f;
			
		}
			
	}
}


void AScatterBall::Init(float BallDamage, float BallScale, bool XSpeedIsPos, bool YSpeedIsPos, UMaterialInterface* Material, bool ZSpeedIsPos)
{

			if(Mesh)
			{
				Mesh->SetWorldScale3D(FVector(BallScale));
				
				if(Material)
				Mesh->SetMaterial(0, Material);

				Mesh->OnComponentBeginOverlap.AddDynamic(this, &AScatterBall::OnOverlapBegin); 
				if(XSpeedIsPos)
					SpeedX = FMath::RandRange(5.f, 2.f);
				else
				{
					SpeedX = FMath::RandRange(-5.f, -2.f);
				}

				if(YSpeedIsPos)
					SpeedY = FMath::RandRange(5.f, 2.f);
				else
				{
					SpeedY = FMath::RandRange(-5.f, -2.f);
				}
		
				if(ZSpeedIsPos)
				SpeedZ = FMath::RandRange(1.f, 10.f);
				else
				SpeedZ = FMath::RandRange(-1.f, -10.f);


				
				Damage = BallDamage;
				FVector Origin;
				FVector BoxExtent;
				GetActorBounds(false, Origin, BoxExtent);

				float Radius;

				if(BoxExtent.X > BoxExtent.Y) Radius = BoxExtent.X;
				else Radius = BoxExtent.Y;
			
				TriggerCapsule->InitCapsuleSize(Radius, BoxExtent.Z+5.f);;
			}
}