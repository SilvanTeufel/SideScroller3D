// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.
#include "Controller/CameraControllerBase.h"


ACameraControllerBase::ACameraControllerBase()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(true);
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ACameraControllerBase::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("CameraControllerBase BEGINPLAY!!!!! "));
	Super::BeginPlay();
	HUDBase = Cast<AHUDBase>(GetHUD());
	CameraBase = Cast<ACameraBase>(GetPawn());
	GetViewPortScreenSizes(1);
	
	if(LockAlwaysToCharacter)
	ToggleLockCamToCharacter();
	
}

void ACameraControllerBase::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void ACameraControllerBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CameraBaseMachine(DeltaSeconds);
}

void ACameraControllerBase::GetViewPortScreenSizes(int x)
{
	switch (x)
	{
	case 1:
		{
			GetViewportSize(CameraBase->ScreenSizeX, CameraBase->ScreenSizeY);
		}
		break;
	case 2:
		{
			CameraBase->ScreenSizeX = GSystemResolution.ResX;
			CameraBase->ScreenSizeY = GSystemResolution.ResY;
		}
		break;
	}
}


FVector ACameraControllerBase::GetCameraPanDirection() {
	float MousePosX = 0;
	float MousePosY = 0;
	float CamDirectionX = 0;
	float CamDirectionY = 0;

	GetMousePosition(MousePosX, MousePosY);

	const float CosYaw = FMath::Cos(CameraBase->YawValue*PI/180);
	const float SinYaw = FMath::Sin(CameraBase->YawValue*PI/180);
	
	if (MousePosX <= CameraBase->Margin)
	{
		CamDirectionY = -CosYaw;
		CamDirectionX = SinYaw;
	}
	if (MousePosY <= CameraBase->Margin)
	{
		CamDirectionX = CosYaw;
		CamDirectionY = SinYaw;
	}
	if (MousePosX >= CameraBase->ScreenSizeX - CameraBase->Margin)
	{
		CamDirectionY = CosYaw;
		CamDirectionX = -SinYaw;
	}
	if (MousePosY >= CameraBase->ScreenSizeY - CameraBase->Margin)
	{
		CamDirectionX = -CosYaw;
		CamDirectionY = -SinYaw;
	}
	
	return FVector(CamDirectionX, CamDirectionY, 0);
}


void ACameraControllerBase::CameraBaseMachine(float DeltaTime)
{
	//HUDBase->MoveActorsThroughWayPoints(MovingActors);
	FVector PanDirection = GetCameraPanDirection();
	
	if(CameraBase)
	{
		switch (CameraBase->GetCameraState())
		{
		case CameraData::ZoomOutPosition:
			{
				ZoomOutToPosition = true;
				//UE_LOG(LogTemp, Warning, TEXT("ZoomToPosition"));
				
				CameraBase->ZoomOutToPosition(CameraBase->ZoomOutPosition);

			}
			break;
		case CameraData::ZoomInPosition:
			{
				ZoomOutToPosition = false;
				ZoomInToPosition = true;
				//UE_LOG(LogTemp, Warning, TEXT("ZoomInPosition"));
				
				if(LockCameraToCharacter) CameraBase->SetCameraState(CameraData::LockOnCharacter);
				
				if(CameraBase->ZoomInToPosition(CameraBase->ZoomPosition))
				{
					ZoomInToPosition = false;
					 CameraBase->SetCameraState(CameraData::LockOnCharacter);
				}
			}
			break;
		case CameraData::LockOnCharacter:
			{
				//UE_LOG(LogTemp, Warning, TEXT("LockOnCharacter"));
				LockCamToCharacter();
			}
			break;
		default:
			{
				//UE_LOG(LogTemp, Warning, TEXT("default"));
				CameraBase->SetCameraState(CameraData::ZoomInPosition);
			}
			break;
		}
	}
}

void ACameraControllerBase::ToggleLockCamToCharacter()
{
		LockCameraToCharacter = !LockCameraToCharacter;
	
		if(LockCameraToCharacter)
		{
			CameraBase->SetCameraState(CameraData::LockOnCharacter);
		}
		else
		{
			CameraBase->SetCameraState(CameraData::ZoomInPosition);
		}
}
void ACameraControllerBase::UnlockCamFromCharacter()
{
	FHitResult Hit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, false, Hit);

	if (Hit.bBlockingHit)
	{
		AUnitBase* CharacterFound = Cast<AUnitBase>(Hit.GetActor());

		if(!CharacterFound){
			CameraBase->SetCameraState(CameraData::ZoomInPosition);
			LockCameraToCharacter = false;
		}
	}
	
}

void ACameraControllerBase::LockCamToCharacter()
{
	if( SelectedUnits.Num())
	{
		FVector SelectedActorLocation = SelectedUnits[0]->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("SelectedUnits[0]->UnitIndex: %d"), SelectedUnits[0]->UnitIndex);
		CameraBase->LockOnUnit(SelectedUnits[0]);
		
		if(!CamIsRotatingLeft && (CamIsRotatingRight || !CameraBase->IsCameraInAngle()))
		{
			CamIsRotatingLeft = false;
			if(CameraBase->RotateCamRight(!LockCameraToCharacter)) CamIsRotatingRight = false;
			SetControlRotation(FRotator(0.f, CameraBase->YawValue, 0.f)); 
		}
		if(CamIsRotatingLeft)
		{
			CamIsRotatingRight = false;
			if(CameraBase->RotateCamLeft(!LockCameraToCharacter)) CamIsRotatingLeft = false;
			SetControlRotation(FRotator(0.f, CameraBase->YawValue, 0.f));
		}
		
		if(CamIsZoomingIn)
		{
			CameraBase->ZoomIn();
		}else if(CamIsZoomingOut)
			CameraBase->ZoomOut();
		else if(ZoomOutToPosition)
		{
			CameraBase->ZoomOutToPosition(CameraBase->ZoomOutPosition, SelectedActorLocation);
		}
		else if(ZoomInToPosition && CameraBase->ZoomInToPosition(CameraBase->ZoomPosition,SelectedActorLocation))
		{
			ZoomInToPosition = false;
		}
		else if(!ZoomInToPosition && CameraBase->IsCharacterDistanceTooLow(CameraBase->ZoomPosition, SelectedActorLocation) && !CamIsZoomingIn && !HoldZoomOnLockedCharacter)
		{
				CameraBase->ZoomOutToPosition(CameraBase->ZoomPosition, SelectedActorLocation);
				CameraBase->CameraDistanceToCharacter = (CameraBase->GetActorLocation().Z - SelectedUnits[0]->GetActorLocation().Z);
		}
	}else if(!LockAlwaysToCharacter)
	{
		LockCameraToCharacter = !LockCameraToCharacter;
		CameraBase->SetCameraState(CameraData::ZoomInPosition);
	}
}

void ACameraControllerBase::LockZDistanceToCharacter()
{
	if(ZoomInToPosition == false &&
		ZoomOutToPosition == false &&
		CamIsZoomingIn == false &&
		CamIsZoomingOut == false &&
		CameraBase &&
		SelectedUnits.Num())
	{
		
		const FVector SelectedActorLocation = SelectedUnits[0]->GetActorLocation();
		const FVector CameraBaseLocation = CameraBase->GetActorLocation();
		
		const float NewCameraDistanceToCharacter = (CameraBaseLocation.Z - SelectedActorLocation.Z);
		float ZChange = CameraBase->CameraDistanceToCharacter - NewCameraDistanceToCharacter;
		
		const float CosYaw = FMath::Cos(CameraBase->YawValue*PI/180);
		const float SinYaw = FMath::Sin(CameraBase->YawValue*PI/180);
		const FVector NewPawnLocation = FVector(SelectedActorLocation.X - CameraBase->CameraDistanceToCharacter * 0.7*CosYaw, SelectedActorLocation.Y - CameraBase->CameraDistanceToCharacter * 0.7*SinYaw, CameraBaseLocation.Z+ZChange);

		CameraBase->SetActorLocation(NewPawnLocation);
		CameraBase->CameraDistanceToCharacter = (CameraBase->GetActorLocation().Z - SelectedUnits[0]->GetActorLocation().Z);
	}
}
