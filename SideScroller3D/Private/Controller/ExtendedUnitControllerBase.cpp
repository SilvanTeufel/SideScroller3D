// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Controller/ExtendedUnitControllerBase.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Kismet/KismetMathLibrary.h"

AExtendedUnitControllerBase::AExtendedUnitControllerBase()
{
}

void AExtendedUnitControllerBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ExtendedUnitControllerBase BEGINPLAY!!!!! "));
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
	case UnitData::Idle:
		{
			UE_LOG(LogTemp, Warning, TEXT("Idle!! "));
			/*if(abs(ExtendedUnitVelocity.Z) > 1.f )
			{
				ExtendedUnitBase->SetUnitState(UnitData::Jump);
			}else */if(abs(ExtendedUnitVelocity.Z) <= 1.f)
			{
				ExtendedUnitBase->DeSpawnGlider();
			}
			CreateJumpColor(ExtendedUnitBase);
		}
		break;
	case UnitData::Attack:
	case UnitData::Chase:
	case UnitData::Pause:
		{
			UE_LOG(LogTemp, Warning, TEXT("Pause!! "));
			CreateJumpColor(ExtendedUnitBase);
		}
		break;
	case UnitData::TripleJump:
		{
			UE_LOG(LogTemp, Warning, TEXT("TripleJump!! "));
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
			UE_LOG(LogTemp, Warning, TEXT("Jump!! "));
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
