// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Actors/ScatterMine.h"
#include "Components/CapsuleComponent.h"
#include "Characters/Unit/UnitBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AScatterMine::AScatterMine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // QueryAndPhysics

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Is DamageMani Capsule"));
	TriggerCapsule->InitCapsuleSize(1.f, 1.0f);;
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AScatterMine::OnOverlapBegin); 
	
}

// Called when the game starts or when spawned
void AScatterMine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AScatterMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime+=DeltaTime;
	
	if(LifeTime >= MaxLifeTime){
		Destroy(true, false);
	}
}

void AScatterMine::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if(OtherActor)
	{
		AUnitBase* UnitToHit = Cast<AUnitBase>(OtherActor);

		if(UnitToHit && UnitToHit->TeamId != TeamId)
		{
			SpawnScatterBalls();
			Destroy(true, false);
		}
			
	}
}


void AScatterMine::Init(AActor* TargetActor, AActor* ShootingActor, FVector LocationToSpawn)
{
	Target = TargetActor;
	
	AExtendedUnitBase* ShootingUnitExtended = Cast<AExtendedUnitBase>(ShootingActor);
	


		if(ShootingUnitExtended)
		{
			TeamId = ShootingUnitExtended->TeamId;
			FVector Origin;
			FVector BoxExtent;
			GetActorBounds(false, Origin, BoxExtent);

			float Radius;

			if(BoxExtent.X > BoxExtent.Y) Radius = BoxExtent.X;
			else Radius = BoxExtent.Y;
			
			TriggerCapsule->InitCapsuleSize(Radius, BoxExtent.Z+1.f);;
			SetActorLocation(LocationToSpawn+SpawnOffset);
		}
	
}

void AScatterMine::SpawnScatterBalls()
{


		if(!SpawnComplete)
		{
			FVector MineLocation = GetActorLocation();


	
			FTransform Transform;
			Transform.SetLocation(MineLocation);
			Transform.SetRotation(FQuat(FRotator::ZeroRotator));
			Transform.SetScale3D(FVector(1.f,1.f,1.f));
	
			for(int i = 0; i < ScatterBallCount; i++)
			{
				ScatterBalls.Emplace(Cast<AScatterBall>
							(UGameplayStatics::BeginDeferredActorSpawnFromClass
							(this, AScatterBall::StaticClass(), Transform,  ESpawnActorCollisionHandlingMethod::AlwaysSpawn)));

				if (ScatterBalls[i] != nullptr)
				{
					const bool XSpeedIsPositive = 0 + (rand() % (1 - 0 + 1)) == 1;
					const bool YSpeedIsPositive = 0 + (rand() % (1 - 0 + 1)) == 1;
					ScatterBalls[i]->Init(Damage, ScaleScatterBall, XSpeedIsPositive, YSpeedIsPositive, ScatterBallMaterial, ZSpeedIsPos);
					UGameplayStatics::FinishSpawningActor(ScatterBalls[i], Transform);
				}
			}
			SpawnComplete = true;
		}	
	
}