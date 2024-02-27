// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Controller/ControllerBase.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Controller/MouseBotControllerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "NavMesh/NavMeshPath.h"
#include "Net/UnrealNetwork.h"


AControllerBase::AControllerBase() {
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
}


void AControllerBase::BeginPlay() {
		UE_LOG(LogTemp, Warning, TEXT("ControllerBase BEGINPLAY!!!!! "));
		HUDBase = Cast<AHUDBase>(GetHUD());
		CameraBase = Cast<ACameraBase>(GetPawn());
}


void AControllerBase::SetupInputComponent() {

	Super::SetupInputComponent();

}

void AControllerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AControllerBase, CameraBase);
	DOREPLIFETIME(AControllerBase, IsShiftPressed);
	DOREPLIFETIME(AControllerBase, AttackToggled);
	DOREPLIFETIME(AControllerBase, IsStrgPressed);
	DOREPLIFETIME(AControllerBase, IsSpacePressed);
	DOREPLIFETIME(AControllerBase, AltIsPressed);
	DOREPLIFETIME(AControllerBase, LeftClickIsPressed);
	DOREPLIFETIME(AControllerBase, LockCameraToUnit);
	DOREPLIFETIME(AControllerBase, SelectableTeamId);
}


void AControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!SelectedUnits.Num())
	{
		SelectedUnits = HUDBase->SelectedUnits;

		if (SelectedUnits.Num()) {

			AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[0]);

			if(ExtendedUnitBase)
			{
				CameraBase->SetUserWidget(ExtendedUnitBase);
				USkeletalMesh* NewWeaponMeshIndicator = ExtendedUnitBase->GetWeaponMesh();
				if(NewWeaponMeshIndicator)
					CameraBase->ChangeWeaponIndicator(NewWeaponMeshIndicator);
			}
		}
	}

	if(QIsPressed)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
			if(ExtendedUnitBase && ExtendedUnitBase->JumpLoad < ExtendedUnitBase->MaxJumpLoad)	
				ExtendedUnitBase->JumpLoad+=ExtendedUnitBase->JumpLoadIncrement*DeltaSeconds;
		}
	}
	
	//if(CameraBase)
		//FixAxis(!CameraBase->SwitchAxis);

}

void AControllerBase::ShiftPressed()
{
	IsShiftPressed = true;
}

void AControllerBase::ShiftReleased()
{
	IsShiftPressed = false;
}

void AControllerBase::LeftClickPressed()
{


}

void AControllerBase::LeftClickReleased()
{

}


void AControllerBase::RightClickPressed()
{
	AttackToggled = false;
	if (IsShiftPressed) {
		for (int32 i = 0; i < SelectedUnits.Num(); i++) {
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead) {
				FHitResult Hit;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
				FVector RunLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);
				DrawDebugSphere(GetWorld(), RunLocation, 15, 5, FColor::Green, false, 1.5, 0, 1);
				if(!SelectedUnits[i]->RunLocationArray.Num())
				{
					SelectedUnits[i]->RunLocation = RunLocation;
					SelectedUnits[i]->SetUnitState(UnitData::Run);
				}
				
				SelectedUnits[i]->RunLocationArray.Add(RunLocation);
				SelectedUnits[i]->UnitStatePlaceholder = UnitData::Idle;
				SelectedUnits[i]->UnitsToChase.Empty();
				SelectedUnits[i]->UnitToChase = nullptr;
				SelectedUnits[i]->ToggleUnitDetection = false;
			}
			
		}
	}
	else {
		for (int32 i = 0; i < SelectedUnits.Num(); i++) {
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead) {
				FHitResult Hit;
				GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
				FVector RunLocation = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

				
				DrawDebugSphere(GetWorld(), RunLocation, 15, 5, FColor::Green, false, 1.5, 0, 1);
				
				if(SelectedUnits[i]->GetUnitState() != UnitData::Run)
				SelectedUnits[i]->SetWalkSpeed(0.f);
				
				SelectedUnits[i]->SetUnitState(UnitData::Run);
				SelectedUnits[i]->RunLocationArray.Empty();
				SelectedUnits[i]->RunLocationArrayIterator = 0;
				SelectedUnits[i]->RunLocation = RunLocation;
				SelectedUnits[i]->UnitStatePlaceholder = UnitData::Idle;
				SelectedUnits[i]->ToggleUnitDetection = false;
			}
		}
	}
}



void AControllerBase::APressed()
{
	if(!AttackToggled)
	{
		AttackToggled = true;
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead)
			{
				if(SelectedUnits[i])
				{
					SelectedUnits[i]->SetWalkSpeed(0);	
					SelectedUnits[i]->ToggleUnitDetection = true;
				}
			}
		}
	}
}

void AControllerBase::AReleased()
{

		AttackToggled = false;
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			if (SelectedUnits[i] && SelectedUnits[i]->UnitState != UnitData::Dead)
			{
				if(SelectedUnits[i])
				{
					SelectedUnits[i]->ToggleUnitDetection = false;
				}
			}
		}
}

void AControllerBase::SReleased()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		if(ExtendedUnitBase)
		{
			ExtendedUnitBase->ActualWeaponId < ExtendedUnitBase->Weapons.Num() - 1 ? ExtendedUnitBase->ActualWeaponId += 1 : ExtendedUnitBase->ActualWeaponId = 0;
			ExtendedUnitBase->ChangeWeapon(ExtendedUnitBase->Weapons[ExtendedUnitBase->ActualWeaponId]);
		}
	}

	if (SelectedUnits.Num()) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[0]);

		if(ExtendedUnitBase)
		{
			USkeletalMesh* NewWeaponMeshIndicator = ExtendedUnitBase->GetWeaponMesh();
			if(NewWeaponMeshIndicator)
				CameraBase->ChangeWeaponIndicator(NewWeaponMeshIndicator);
		}
	}
}

void AControllerBase::RPressed()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[0]);
		if(ExtendedUnitBase)
		{
			if(ExtendedUnitBase->GetUnitState() != UnitData::Reload && ExtendedUnitBase->GetUnitState() != UnitData::Pause)
			{
				ExtendedUnitBase->SetWalkSpeed(0);
				ExtendedUnitBase->UnitControlTimer = 0;
				ExtendedUnitBase->ReloadWeapon();
				
			}
		}
	}
}

void AControllerBase::FPressed()
{
	AttackToggled = false;
	/*
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(!ExtendedUnitBase)
			return;
		
			FHitResult Hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
			ExtendedUnitBase->TeleportLocation = Hit.Location;
			
			ExtendedUnitBase->SetWalkSpeed(0);
			ExtendedUnitBase->UnitControlTimer = 0;
			ExtendedUnitBase->SetUnitState(UnitData::TeleportStart);
	
	}*/
}

void AControllerBase::CPressed()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(!ExtendedUnitBase)
			return;
		
			ExtendedUnitBase->ToggleUnitDetection = false;
			ExtendedUnitBase->UnitControlTimer = 0;
			ExtendedUnitBase->SetUnitState(UnitData::PrepareScatterMine);

	}
}


void AControllerBase::EPressed()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		ExtendedUnitBase->ToggleShield();
	}
}

void AControllerBase::XPressed()
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {

		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(!ExtendedUnitBase)
			return;
		
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		ExtendedUnitBase->ToggleUnitDetection = false;
		ExtendedUnitBase->MouseBotSpawnLocation = Hit.Location;
		ExtendedUnitBase->UnitControlTimer = 0;
		ExtendedUnitBase->SetUnitState(UnitData::PrepareMouseBot);
	}
}

void AControllerBase::QPressed()
{
	QIsPressed = true;
	
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		
		if(ExtendedUnitBase->JumpCounter == 1 && ExtendedUnitBase->JumpCounter < ExtendedUnitBase->MaxJumps)
		{
			ExtendedUnitBase->JumpCounter++;
			FVector DirectionVector = FVector(0.f,0.f,1.f);
			DirectionVector = FVector(DirectionVector.X*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->MaxJumpLoad), DirectionVector.Y*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->MaxJumpLoad), DirectionVector.Z*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->MaxJumpLoad));
			ExtendedUnitBase->LaunchCharacter(DirectionVector,false, false);
			ExtendedUnitBase->SetUnitState(UnitData::Jump);
		}
	}
}

void AControllerBase::QReleased()
{
	
	QIsPressed = false;
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		
		if(ExtendedUnitBase->JumpCounter == 0 && ExtendedUnitBase->JumpCounter <= ExtendedUnitBase->MaxJumps)
		{
		 	ExtendedUnitBase->JumpCounter++;
			FVector DirectionVector = FVector(0.f,0.f,1.f);
			DirectionVector = FVector(DirectionVector.X*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->JumpLoad), DirectionVector.Y*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->JumpLoad), DirectionVector.Z*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->JumpLoad));
		
			ExtendedUnitBase->LaunchCharacter(DirectionVector,false, false);
			ExtendedUnitBase->SetUnitState(UnitData::Jump);
		}
		ExtendedUnitBase->JumpLoad = 0.f;
	}
}


void AControllerBase::TripleJump(FVector JumpDirection)
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		if(ExtendedUnitBase->JumpCounter == 2 && ExtendedUnitBase->JumpCounter < ExtendedUnitBase->MaxJumps)
		{
			ExtendedUnitBase->JumpCounter++;
	

			FVector UnitLocation = ExtendedUnitBase->GetActorLocation();
			
			FVector DirectionVector = UKismetMathLibrary::GetDirectionUnitVector(UnitLocation, FVector(UnitLocation.X+1000.f*JumpDirection.Y, UnitLocation.Y+1000.f*JumpDirection.X, UnitLocation.Z+1000.f*JumpDirection.Z));
			ExtendedUnitBase->AddMovementInput(DirectionVector, ExtendedUnitBase->Attributes->GetRunSpeedScale()*abs(JumpDirection.X+JumpDirection.Y));
			DirectionVector = FVector(DirectionVector.X*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->MaxJumpLoad)/2, DirectionVector.Y*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->MaxJumpLoad)/2, DirectionVector.Z*(ExtendedUnitBase->JumpLoadOffset+ExtendedUnitBase->MaxJumpLoad));
			ExtendedUnitBase->LaunchCharacter(DirectionVector,false, false);
			ExtendedUnitBase->SetUnitState(UnitData::TripleJump);
		}
	}
	
}

void AControllerBase::SetDropJumpMine(bool DropMine)
{

	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		if(ExtendedUnitBase)
		{
			ExtendedUnitBase->DropJumpMine = DropMine;
		}
	}
	
}

void AControllerBase::JumpCamera()
{
	if (CameraBase && (IsStrgPressed || IsSpacePressed))
	{
		FHitResult Hit;
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
		CameraBase->JumpCamera(Hit);

	}
}


void AControllerBase::StrgPressed() {
	IsStrgPressed = true;
}

void AControllerBase::StrgReleased() {
	IsStrgPressed = false;
}

void AControllerBase::ZoomIn()
{
	if (CameraBase && IsStrgPressed)
	{
		CameraBase->ZoomIn();
	}
}

void AControllerBase::ZoomOut()
{
	if (CameraBase && IsStrgPressed)
	{
		CameraBase->ZoomOut();
	}
}


void AControllerBase::MouseBotStateMachine(float DeltaSeconds)
{
	TArray <AMouseBotBase*> MouseBots = HUDBase->MouseBots;
	
	if(MouseBots.Num())
	
		for (int32 i = 0; i < MouseBots.Num(); i++) {
				
			if(MouseBots[i])
				switch (MouseBots[i]->GetAnimState())
				{
			case BotData::Idle_Bot:
				{
					FHitResult Hit;
					GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
					HUDBase->ControllDirectionToMouse(MouseBots[i], Hit);
				}
					break;
			case BotData::Run_Bot:
				{
					FHitResult Hit;
					GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
					HUDBase->ControllDirectionToMouse(MouseBots[i], Hit);
					MouseBots[i]->MoveToLocation = Hit.Location;
				}
					break;
			case BotData::Dead_Bot:
				{
					if(MouseBots[i]->DeathTime > 5.f)
					{
						AMouseBotBase* BotToKill = MouseBots[i];
						HUDBase->RemoveMouseBot(BotToKill);
						BotToKill->KillBot();
					}
								
				}
					break;
			default:
				{
							
				}
					break;
				}
		}
}

void AControllerBase::SetRunLocation_Implementation(AUnitBase* Unit, const FVector& DestinationLocation)
{
	Unit->SetRunLocation(DestinationLocation);
}

void AControllerBase::MoveToLocationUEPathFinding_Implementation(AUnitBase* Unit, const FVector& DestinationLocation)
{

	
	if(!HasAuthority())
	{
		return;
	}
	
	if (!Unit || !Unit->GetCharacterMovement())
	{
		return;
	}

	// Check if we have a valid AI controller for the unit
	AAIController* AIController = Cast<AAIController>(Unit->GetController());
	if (!AIController)
	{
		return;
	}

	// Check if we have a valid navigation system
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem)
	{
		return;
	}
	
	SetRunLocation(Unit, DestinationLocation);
	Unit->UEPathfindingUsed = true;
	// Move the unit to the destination location using the navigation system
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(DestinationLocation);
	MoveRequest.SetAcceptanceRadius(5.0f); // Set an acceptance radius for reaching the destination
	
	FNavPathSharedPtr NavPath;
	
	AIController->MoveTo(MoveRequest, &NavPath);
	
	if(NavPath)
	{
		FNavMeshPath* NavMeshPath = NavPath->CastPath<FNavMeshPath>();
		if (NavMeshPath)
		{
			NavMeshPath->OffsetFromCorners(UEPathfindingCornerOffset);
		}
	}
}

void AControllerBase::AltPressed()
{
	TArray <AMouseBotBase*> MouseBots = HUDBase->MouseBots;
	
	for(int i = 0; i < MouseBots.Num(); i++)
	{
		if(MouseBots[i])
		{
			if(!BotIsToggled)
			{
				MouseBots[i]->SetAnimState(BotData::Run_Bot);
			}else
			{
				MouseBots[i]->SetAnimState(BotData::Idle_Bot);
			}	
		}
	}
	BotIsToggled = !BotIsToggled;
}

void AControllerBase::AltReleased()
{
	AltIsPressed = false;
}

void AControllerBase::SetAxis()
{

		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
			FVector ActorLocation = ExtendedUnitBase->GetActorLocation();
			
			if(ExtendedUnitBase)
			{
	
					ExtendedUnitBase->XAxisFixedPosition = ActorLocation.X;
					ExtendedUnitBase->YAxisFixedPosition = ActorLocation.Y;

			}
		}

}

void AControllerBase::FixAxis(bool IsX)
{

	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);
		FVector ActorLocation = ExtendedUnitBase->GetActorLocation();


		if(ExtendedUnitBase)
		{
			if(IsX)
			{
				if(ActorLocation.X != ExtendedUnitBase->XAxisFixedPosition)
					ExtendedUnitBase->SetActorLocation(FVector(ExtendedUnitBase->XAxisFixedPosition, ActorLocation.Y, ActorLocation.Z));
			}else
			{
				if(ActorLocation.Y != ExtendedUnitBase->YAxisFixedPosition)
					ExtendedUnitBase->SetActorLocation(FVector(ActorLocation.X, ExtendedUnitBase->YAxisFixedPosition, ActorLocation.Z));
			}
		}
	}

}

void AControllerBase::JoystickRunUnit(FVector2D AddDirection)
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++)
	{
		AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnits[i]);

		if(ExtendedUnitBase && ExtendedUnitBase->GetUnitState() != UnitData::TripleJump)
		{
			FVector UnitLocation = ExtendedUnitBase->GetActorLocation();
			
			ExtendedUnitBase->SetUnitState(UnitData::Run);
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(UnitLocation, FVector(UnitLocation.X+1000.f*AddDirection.Y, UnitLocation.Y+1000.f*AddDirection.X, UnitLocation.Z));
			
			ExtendedUnitBase->AddMovementInput(Direction, ExtendedUnitBase->Attributes->GetRunSpeedScale()*abs(AddDirection.X+AddDirection.Y));
			ExtendedUnitBase->UnitStatePlaceholder = UnitData::Idle;
		}
	}
}

void AControllerBase::SetControlerTeamId_Implementation(int Id)
{
	SelectableTeamId = Id;
}
