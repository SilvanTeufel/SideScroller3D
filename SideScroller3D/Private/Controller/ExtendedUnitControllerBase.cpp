// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Controller/ExtendedUnitControllerBase.h"

#include "Actors/SelectedIcon.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Kismet/KismetMathLibrary.h"

AExtendedUnitControllerBase::AExtendedUnitControllerBase()
{
}

void AExtendedUnitControllerBase::BeginPlay()
{
	Super::BeginPlay();
	/*
	AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(GetPawn());

	if(ExtendedUnitBase)
	{
		FVector ActorLocation = ExtendedUnitBase->GetActorLocation();
		ExtendedUnitBase->XAxisFixedPosition = ActorLocation.X;
		ExtendedUnitBase->YAxisFixedPosition = ActorLocation.Y;
	}*/
}



void AExtendedUnitControllerBase::Tick(float DeltaSeconds)
{
	//UnitControlStateMachine(DeltaSeconds);
	ExtendedUnitControlMachine(DeltaSeconds);
}


void AExtendedUnitControllerBase::CreateJumpColor(AExtendedUnitBase* ExtendedUnitBase)
{
	if(ExtendedUnitBase->JumpLoad > 0)
	{
		if(ExtendedUnitBase->JumpCounter == 0)
			ExtendedUnitBase->CreateIconJumpRadius(FVector(5.f, 5.f, 100.f));
		if(ExtendedUnitBase->JumpCounter == 1)
			ExtendedUnitBase->CreateIconJumpRadius(FVector(100.f, 100.f, 5.f));
		if(ExtendedUnitBase->JumpCounter == 2)
			ExtendedUnitBase->CreateIconJumpRadius(FVector(400.f, 200.f, 100.f));
		if(ExtendedUnitBase->JumpCounter == 3)
			ExtendedUnitBase->CreateIconJumpRadius(FVector(100.f, 5.f, 5.f));
	}else if(ExtendedUnitBase->JumpCounter == 1)
		ExtendedUnitBase->CreateColoredIconNormalRadius(FVector(100.f, 100.f, 5.f));
	else if(ExtendedUnitBase->JumpCounter == 2)
		ExtendedUnitBase->CreateColoredIconNormalRadius(FVector(400.f, 200.f, 200.f));
	else if(ExtendedUnitBase->JumpCounter == 3)
		ExtendedUnitBase->CreateColoredIconNormalRadius(FVector(100.f, 5.f, 5.f));
	else
	{
		ExtendedUnitBase->CreateIconNormalRadius();
	}
}


void AExtendedUnitControllerBase::ExtendedUnitControlMachine(float DeltaSeconds)
{
	AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(GetPawn());

	// Jump Counter Timer
	FVector ExtendedUnitVelocity = FVector(0.f,0.f, 0.f);
	if(ExtendedUnitBase)
	{
		if(ExtendedUnitBase->JumpCounter >= 0.f)
		{
			ExtendedUnitBase->JumpCounterTime = (ExtendedUnitBase->JumpCounterTime + DeltaSeconds);
			if(ExtendedUnitBase->JumpCounterTime >= ExtendedUnitBase->JumpCounterResetTime)
			{
				ExtendedUnitBase->JumpCounterTime = 0.f;
				ExtendedUnitBase->JumpCounter = 0.f;
			}	
		}
		ExtendedUnitVelocity = ExtendedUnitBase->GetVelocity();
	}
	//  Jump Counter Timer
	
	if(ExtendedUnitBase)
	switch (ExtendedUnitBase->UnitState)
	{
	case UnitData::Run:
		{
			
			FVector Velocity = ExtendedUnitBase->GetVelocity(); // Annahme, dass GetVelocity() verfügbar ist
			float Speed = Velocity.Size(); // Die Geschwindigkeit als Skalar

			//UE_LOG(LogTemp, Warning, TEXT("Run!! Speed: %f"), Speed);
			//UE_LOG(LogTemp, Warning, TEXT("Run!! "));
			// Überprüfen, ob die Geschwindigkeit einen bestimmten Wert überschreitet
			if(Velocity.Z >= 10.f || Velocity.Z <= -10.f)
			{
				ExtendedUnitBase->SetUnitState(UnitData::Falling);
			}else if (Velocity.X == 0.f && Velocity.Y == 0.f)
			{
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}
		}
		break;
	case UnitData::Idle:
		{
			//UE_LOG(LogTemp, Warning, TEXT("Idle!! "));
			/*if(abs(ExtendedUnitVelocity.Z) > 1.f )
			{
				ExtendedUnitBase->SetUnitState(UnitData::Jump);
			}else *//*if(abs(ExtendedUnitVelocity.Z) <= 1.f)
			{
				ExtendedUnitBase->DeSpawnGlider();
			}
			CreateJumpColor(ExtendedUnitBase);

			*/
		}
		break;
	case UnitData::Falling:
		{
			//UE_LOG(LogTemp, Warning, TEXT("Falling!! "));
			FVector Velocity = ExtendedUnitBase->GetVelocity();
			if(Velocity.Z >= -10.f && Velocity.Z <= 10.f)
			{
				ExtendedUnitBase->SetUnitState(UnitData::Run);
			}
		}break;
	case UnitData::Attack:
		{
			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			//ExtendedUnitBase->SetNextUnitToChase();
			//ExtendedUnitBase->UnitToChase->SetSelected();
			RotateToAttackUnit(ExtendedUnitBase, ExtendedUnitBase->UnitToChase);
		
			if (ExtendedUnitBase->UnitControlTimer > AttackDuration + PauseDuration)
			{
				ExtendedUnitBase->SetUnitState(UnitData::Pause);
			}
			
		}
		break;
	//case UnitData::Chase:
	case UnitData::Pause:
		{
			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			if(ExtendedUnitBase->UnitToChase && ExtendedUnitBase->UnitToChase->GetUnitState() == UnitData::Dead) {

				if(ExtendedUnitBase->SetNextUnitToChase()) return;

				ExtendedUnitBase->SetUEPathfinding = true;
				ExtendedUnitBase->SetUnitState( ExtendedUnitBase->UnitStatePlaceholder );
				
			} else if (ExtendedUnitBase->UnitControlTimer > PauseDuration)
			{
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}
			//CreateJumpColor(ExtendedUnitBase);
		}
		break;
	case UnitData::TripleJump:
		{
			const FVector UnitLocation = ExtendedUnitBase->GetActorLocation();
			
			ExtendedUnitBase->SpawnGlider();

			CreateJumpColor(ExtendedUnitBase);
			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);

			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			if(ExtendedUnitBase->UnitControlTimer > ExtendedUnitBase->JumpMineSpawnDelta && ExtendedUnitBase->DropJumpMine)
			{
				ExtendedUnitBase->UnitControlTimer = 0.f;
				ExtendedUnitBase->ScatterMineSpawnLocation = FVector(UnitLocation.X, UnitLocation.Y, UnitLocation.Z);
				ExtendedUnitBase->SpawnScatterMine(ExtendedUnitBase->UnitToChase, ExtendedUnitBase, false, ExtendedUnitBase->JumpMineClass);
			}

			if(abs(ExtendedUnitVelocity.Z) <= 1.f)
			{
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
				ExtendedUnitBase->DeSpawnGlider();
			}

			
		}
		break;
	case UnitData::Jump:
		{
			//UE_LOG(LogTemp, Warning, TEXT("Jump!! "));
			/*
			CreateJumpColor(ExtendedUnitBase);

			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			FVector Velo = ExtendedUnitBase->GetVelocity();

			if(abs(Velo.Z) <= 1.f )
			{
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}*/

		}
		break;
	case UnitData::Reload:
		{
			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			if (ExtendedUnitBase->UnitControlTimer >= ExtendedUnitBase->ReloadTime) {
				ExtendedUnitBase->UnitControlTimer = 0.f;
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}
		}
		break;
	case UnitData::PrepareScatterMine:
		{
			if(ExtendedUnitBase->Mana < ExtendedUnitBase->MineManaCost)
			{
				ExtendedUnitBase->UnitControlTimer = 0;
				ExtendedUnitBase->SetUnitState(UnitData::NoMana);
				return;
			}
			
			ExtendedUnitBase->CreateMineSpawnRadius();
			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			if (ExtendedUnitBase->UnitControlTimer >= ExtendedUnitBase->ScatterMineCastTime)
			{
				FVector ALocation = ExtendedUnitBase->GetActorLocation();
				
				float vdist = FVector::Dist(ALocation, ExtendedUnitBase->ScatterMineSpawnLocation) / 35;
				
				
				if(vdist > ExtendedUnitBase->MineSpawnRadius)
					ExtendedUnitBase->ScatterMineSpawnLocation = FVector(ALocation.X, ALocation.Y, ALocation.Z);
				
				ExtendedUnitBase->SpawnScatterMine(ExtendedUnitBase->UnitToChase, ExtendedUnitBase, true, ExtendedUnitBase->ScatterMineBaseClass);
				ExtendedUnitBase->UnitControlTimer = 0.f;
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}

		}
		break;
	case UnitData::PrepareMouseBot:
		{
			if(ExtendedUnitBase->Mana < ExtendedUnitBase->BotManaCost)
			{
				ExtendedUnitBase->UnitControlTimer = 0;
				ExtendedUnitBase->SetUnitState(UnitData::NoMana);
				return;
			}
			
			ExtendedUnitBase->CreateMouseBotSpawnRadius();
			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			
			if (ExtendedUnitBase->UnitControlTimer >= ExtendedUnitBase->MouseBotCastTime)
			{
				FVector ALocation = ExtendedUnitBase->GetActorLocation();
				
				float vdist = FVector::Dist(ALocation, ExtendedUnitBase->MouseBotSpawnLocation) / 35;
				
				if(vdist > ExtendedUnitBase->MouseBotRadius)
					ExtendedUnitBase->MouseBotSpawnLocation = FVector(ALocation.X, ALocation.Y, ExtendedUnitBase->MouseBotSpawnLocation.Z);


				ExtendedUnitBase->MouseBotSpawnLocation = FVector( ExtendedUnitBase->MouseBotSpawnLocation.X, ExtendedUnitBase->MouseBotSpawnLocation.Y, ExtendedUnitBase->MouseBotSpawnLocation.Z+500.f);
				ExtendedUnitBase->SpawnMouseBot();
				ExtendedUnitBase->UnitControlTimer = 0.f;
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}

		}
		break;
	case UnitData::NoMana:
		{
			ExtendedUnitBase->UnitControlTimer = (ExtendedUnitBase->UnitControlTimer + DeltaSeconds);
			if(ExtendedUnitBase->UnitControlTimer > ExtendedUnitBase->NoManaTime){
				ExtendedUnitBase->UnitControlTimer = 0;
				ExtendedUnitBase->SetUnitState(UnitData::Idle);
			}
		}
		break;


	default:
		{
			if(ExtendedUnitBase->ToggleUnitDetection)
				ExtendedUnitBase->CreateIconActionRadius();
			else
				ExtendedUnitBase->CreateIconNormalRadius();
		}
		break;
	}	
	
}
