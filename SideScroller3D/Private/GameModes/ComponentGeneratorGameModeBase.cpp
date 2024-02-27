// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "GameModes/ComponentGeneratorGameModeBase.h"
#include "Kismet/GameplayStatics.h"

AComponentGeneratorGameModeBase::AComponentGeneratorGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AComponentGeneratorGameModeBase::EndGame()
{
	
}

void AComponentGeneratorGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemys();
	ComponentSpawn();
}

void AComponentGeneratorGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckSpawnTimer += DeltaTime;
	if(CheckSpawnTimer >= 10.f)
	{
		CheckSpawnTimer = 0.f;
		DespawnDeadEnemys();
		RespawnAllEnemys();
		
	}
}

void AComponentGeneratorGameModeBase::ComponentSpawn()
{
	if (ComponentClass) {
		ComponentNames = GetComponentNames();
		for(int i = 0; i < ComponentNames.Num(); i++)
		{
			if(!ComponentNames[i].IsEmpty())
			{
				const int Frequency = GetComponentFrequency(ComponentNames[i]);
				for(int f = 0; f < Frequency; f++)
				{
					FVector Location = CalcLocation(GetComponentAbsoluteOffset(ComponentNames[i]), GetComponentRandRangeMin(ComponentNames[i]), GetComponentRandRangeMax(ComponentNames[i]));
					FRotator TotalRotation = GetAbsoluteRotationOffset(ComponentNames[i]) + FRandomRotator(FRotator(0.f,0.f,0.f), GetRotationRange(ComponentNames[i]));
					FVector Scale = GetComponentScale(ComponentNames[i]);

					FTransform Transform;
					Transform.SetLocation(Location);
					Transform.SetRotation(FQuat(TotalRotation)); // FRotator::ZeroRotator
					Transform.SetScale3D(Scale);
					
					const ESpawnActorCollisionHandlingMethod C_Method = GetCollisionMethod(ComponentNames[i]);
					const auto MyComponent = Cast<AComponent>
						(UGameplayStatics::BeginDeferredActorSpawnFromClass
						(this, ComponentClass, Transform, C_Method));

					if (MyComponent != nullptr)
					{
						MyComponent->Init(ComponentNames[i], GetComponentMesh(ComponentNames[i]),
							GetComponentMaterial(ComponentNames[i]),
							GetComponentIsMoving(ComponentNames[i]),
							GetComponentMovementRange(ComponentNames[i]),
							GetComponentMovementSpeed(ComponentNames[i]));

						UGameplayStatics::FinishSpawningActor(MyComponent, Transform);
						Components.Emplace(MyComponent);
					}
				}
			}
		}
	}
	
}

void AComponentGeneratorGameModeBase::SpawnEnemys()
{
	
	for(int i = 0; i < UnitCount; i++)
	{
		// Waypointspawn
		const FVector FirstLocation = CalcLocation(UnitOffset, UnitMinRange, UnitMaxRange);
		const FVector SecondLocation = CreateSecondWaypointLocation(FirstLocation);
		FTransform FirstTransform;
		FirstTransform.SetLocation(FirstLocation);
		FirstTransform.SetRotation(FQuat(FRotator::ZeroRotator));

		const auto FirstWayPoint = Cast<AWaypoint>
						(UGameplayStatics::BeginDeferredActorSpawnFromClass
						(this, WaypointClass, FirstTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

	
		FTransform SecondTransform;
		SecondTransform.SetLocation(SecondLocation);
		SecondTransform.SetRotation(FQuat(FRotator::ZeroRotator));
		const auto SecondWayPoint = Cast<AWaypoint>
						(UGameplayStatics::BeginDeferredActorSpawnFromClass
						(this, WaypointClass, SecondTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));


		FTransform EnemyTransform;
		 EnemyTransform.SetLocation(FVector(FirstLocation.X, FirstLocation.Y, 100.f));
		
		const auto UnitBase = Cast<AUnitBase>
			(UGameplayStatics::BeginDeferredActorSpawnFromClass
			(this, UnitBaseClass, EnemyTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn));

		if (FirstWayPoint != nullptr && SecondWayPoint != nullptr && UnitBase != nullptr)
		{
			// Set NextWaypoint
			FirstWayPoint->NextWaypoint = SecondWayPoint;
			SecondWayPoint->NextWaypoint = FirstWayPoint;
			UnitBase->NextWaypoint = SecondWayPoint;



		
			UGameplayStatics::FinishSpawningActor(FirstWayPoint, FirstTransform);
			FirstWaypoints.Emplace(FirstWayPoint);
		
			UGameplayStatics::FinishSpawningActor(SecondWayPoint, SecondTransform);
			SecondWaypoints.Emplace(SecondWayPoint);

			UGameplayStatics::FinishSpawningActor(UnitBase, EnemyTransform);
			UnitBases.Emplace(UnitBase);
		
		}
	}
	
	// Enemyspawn
}

void AComponentGeneratorGameModeBase::DespawnDeadEnemys()
{	
	
		for(int i = 0; i < UnitBases.Num(); i++)
		{
			if(UnitBases[i])
			if(UnitBases[i]->GetUnitState() == UnitData::Dead)
			{
				AWaypoint* FirstWaypoint = FirstWaypoints[i];
				AWaypoint* SecondWaypoint  = SecondWaypoints[i];
				AUnitBase* UnitBase = UnitBases[i];
				UnitBases.Remove(UnitBase);
				FirstWaypoints.Remove(FirstWaypoint);
				SecondWaypoints.Remove(SecondWaypoint);
				UnitBase->Destroy(true, false);
				FirstWaypoint->Destroy(true, false);
				SecondWaypoint->Destroy(true, false);
			}
		}
	
}

void AComponentGeneratorGameModeBase::RespawnAllEnemys()
{
	
	if(UnitBases.Num() <= UnitCountToRespawn)
	{
		// One Enemy Dies so trigger
		for(int i = 0; i < UnitBases.Num(); i++)
		{
				AWaypoint* FirstWaypoint = FirstWaypoints[i];
				AWaypoint* SecondWaypoint  = SecondWaypoints[i];
				AUnitBase* EnemyBase = UnitBases[i];
				UnitBases.Remove(EnemyBase);
				FirstWaypoints.Remove(FirstWaypoint);
				SecondWaypoints.Remove(SecondWaypoint);
				EnemyBase->Destroy(true, false);
				FirstWaypoint->Destroy(true, false);
				SecondWaypoint->Destroy(true, false);
		}
		SpawnEnemys();
	}
}

FVector AComponentGeneratorGameModeBase::CreateSecondWaypointLocation(FVector FirstLocation)
{
	int MultiplierA;
	const float randChooserA = FMath::RandRange(1, 10);
	if(randChooserA <= 5)
	{
		MultiplierA = -1;
	}else
	{
		MultiplierA = 1;
	}

	int MultiplierB;
	const float randChooserB = FMath::RandRange(1, 10);
	if(randChooserB <= 5)
	{
		MultiplierB = -1;
	}else
	{
		MultiplierB = 1;
	}
				
	const float RandomOffsetX = FMath::RandRange(500.f, 1000.f);
	const float RandomOffsetY = FMath::RandRange(500.f, 1000.f);
	
				
	const float X = RandomOffsetX*MultiplierA+FirstLocation.X; 
	const float Y = RandomOffsetY*MultiplierB+FirstLocation.Y; 
	const float Z = FirstLocation.Z;
				
	return FVector(X, Y, Z);
}


FVector AComponentGeneratorGameModeBase::CalcLocation(FVector Offset, FVector MinRange, FVector MaxRange)
{
	int MultiplierA;
	const float randChooserA = FMath::RandRange(1, 10);
	if(randChooserA <= 5)
	{
		MultiplierA = -1;
	}else
	{
		MultiplierA = 1;
	}

	int MultiplierB;
	const float randChooserB = FMath::RandRange(1, 10);
	if(randChooserB <= 5)
	{
		MultiplierB = -1;
	}else
	{
		MultiplierB = 1;
	}
				
	const float RandomOffsetX = FMath::RandRange(MinRange.X, MaxRange.X);
	const float RandomOffsetY = FMath::RandRange(MinRange.Y, MaxRange.Y);
	const float RandomOffsetZ = FMath::RandRange(MinRange.Z, MaxRange.Z);
				
	const float X = RandomOffsetX*MultiplierA+Offset.X; 
	const float Y = RandomOffsetY*MultiplierB+Offset.Y; 
	const float Z = RandomOffsetZ+Offset.Z;
				
	return FVector(X, Y, Z);
}



FRotator AComponentGeneratorGameModeBase::FRandomRotator(FRotator Min, FRotator Max)
{
	const float Pitch = FMath::FRandRange(Min.Pitch, Max.Pitch);
	const float Yaw = FMath::FRandRange(Min.Yaw, Max.Yaw);
	const float Roll = FMath::FRandRange(Min.Roll, Max.Roll);
	return FRotator(Pitch, Yaw, Roll);
}

FRotator AComponentGeneratorGameModeBase::GetRotationRange(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->RotationRange;
	}
	return FRotator(0.f,0.f,0.f);
}

FRotator AComponentGeneratorGameModeBase::GetAbsoluteRotationOffset(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->AbsoluteRotationOffset;
	}
	return FRotator(0.f,0.f,0.f);
	
}


UStaticMesh* AComponentGeneratorGameModeBase::GetComponentMesh(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name); // "AR-15DT"
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true); // AR-15 // AS-VAL
		return ComponentData->Mesh;
	}
	return nullptr;
}

FVector AComponentGeneratorGameModeBase::GetComponentScale(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->Scale;
	}
	return FVector(1.f,1.f,1.f);
}


FVector AComponentGeneratorGameModeBase::GetComponentRandRangeMin(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->RandomRangeMin;
	}
	return FVector(1000.f,1000.f,1000.f);
}

FVector AComponentGeneratorGameModeBase::GetComponentRandRangeMax(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->RandomRangeMax;
	}
	return FVector(5000.f,5000.f,5000.f);
}



FVector AComponentGeneratorGameModeBase::GetComponentAbsoluteOffset(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->AbsoluteOffset;
	}
	return FVector(0.f,0.f,0.f);
}

bool AComponentGeneratorGameModeBase::GetComponentIsMoving(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->IsMoving;
	}
	return false;
}

FVector AComponentGeneratorGameModeBase::GetComponentMovementSpeed(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->MovementSpeed;
	}
	return FVector(1.f,1.f,0.f);
}

float AComponentGeneratorGameModeBase::GetComponentMovementRange(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->MovementRange;
	}
	return 0.f;
}



UMaterial* AComponentGeneratorGameModeBase::GetComponentMaterial(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->Material;
	}
	return nullptr;
}

int AComponentGeneratorGameModeBase::GetComponentFrequency(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->Frequency;
	}
	return 1;
}

ESpawnActorCollisionHandlingMethod AComponentGeneratorGameModeBase::GetCollisionMethod(FString Name)
{
	if (ComponentDataTable)
	{
		static const FString PString = FString(Name);
		ComponentData = ComponentDataTable->FindRow<FComponentData>(FName(Name), PString, true);
		return ComponentData->CollisionMethod;
	}else
	{
		return ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	}
}

TArray<FString> AComponentGeneratorGameModeBase::GetComponentNames()
{
	if (ComponentDataTable)
	{
		TArray<FName> Names = ComponentDataTable->GetRowNames();
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

