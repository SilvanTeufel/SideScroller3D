// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Characters/Camera/CameraBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Controller/Input/EnhancedInputComponentBase.h"
#include "Controller/Input/GameplayTags.h"
#include "UnrealEngine.h"
#include "Widgets/UnitBaseActionBar.h"
#include "EnhancedInputSubsystems.h"
#include "Controller/CameraControllerBase.h"
#include "Kismet/KismetMathLibrary.h"
/*
ACameraBase::ACameraBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CreateCameraComp();

	if (RootComponent == nullptr) {
		RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	}

	ControlWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("ControlWidget"));
	ControlWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	ActionWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("ActionWidgetComp"));
	ActionWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	SetActorEnableCollision(false);
} */

// Called when the game starts or when spawned
void ACameraBase::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("CameraBase BEGINPLAY!!!!! "));
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		
		// Get the Enhanced Input Local Player Subsystem from the Local Player related to our Player Controller.
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// PawnClientRestart can run more than once in an Actor's lifetime, so start by clearing out any leftover mappings.
			Subsystem->ClearAllMappings();

			// Add each mapping context, along with their priority values. Higher values outprioritize lower values.
			Subsystem->AddMappingContext(MappingContext, MappingPriority);
		}
		
	}
	
	SpawnControlWidget();
	SpawnWeaponIndicator();
	SpawnActionWidget();
	ShowControlWidget();

}

// Called every frame
void ACameraBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ACameraBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


}

void ACameraBase::CreateCameraComp()
{
	RootScene= RootComponent;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCameraBaseCapsule()); // RootScene // GetCapsuleComponent()
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->SetRelativeRotation(SpringArmRotator);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArm);
}


void ACameraBase::SpawnControlWidget()
{
	FTransform SpellTransform;
	SpellTransform.SetLocation(FVector(500, 0, 0));
	SpellTransform.SetRotation(FQuat(FRotator::ZeroRotator));


	if (ControlWidgetComp) {
		FRotator NewRotation = ControlWidgetRotation;
		FQuat QuatRotation = FQuat(NewRotation);
		ControlWidgetComp->SetRelativeRotation(QuatRotation, false, 0, ETeleportType::None);
		ControlWidgetComp->SetRelativeLocation(ControlWidgetLocation);
	}
}



void ACameraBase::PanMoveCamera(const FVector& NewPanDirection) {
	if (NewPanDirection != FVector::ZeroVector) {
		AddActorWorldOffset(NewPanDirection * GetActorLocation().Z * 0.001);
	}
}

void ACameraBase::ZoomOut() {
	AddActorWorldOffset(FVector(0, 0, 0.3) * CamSpeed);
}

void ACameraBase::ZoomIn() {
	if(GetActorLocation().Z > 0.f)
		AddActorWorldOffset(FVector(0, 0, -0.3) * CamSpeed);
}


bool ACameraBase::RotateCamLeft(bool UseCamRotationOffset)
{
	
	YawValue += 1.f;
	if(UseCamRotationOffset)
	{
		if (YawValue <= CameraAngles[0] + RotationDegreeStep) {
			AddActorWorldOffset(FVector(+CamRotationOffset, -CamRotationOffset, 0));
		}
		else if (YawValue <= CameraAngles[1] + RotationDegreeStep) {
			AddActorWorldOffset(FVector(+CamRotationOffset, +CamRotationOffset, 0));
		}
		else if (YawValue <= CameraAngles[2] + RotationDegreeStep) {
			AddActorWorldOffset(FVector(-CamRotationOffset, +CamRotationOffset, 0));
		}
		else if (YawValue <= CameraAngles[3] + RotationDegreeStep) {
			AddActorWorldOffset(FVector(-CamRotationOffset, -CamRotationOffset, 0));
		}
	}
	if (YawValue == 360) YawValue = 0.f;
	if (YawValue == CameraAngles[0] || YawValue == CameraAngles[3] || YawValue == CameraAngles[2] || YawValue == CameraAngles[1]) return true;
	
	return false;
}

bool ACameraBase::RotateCamRight(bool UseCamRotationOffset)
{
	
	YawValue -= 1.f;
	if(UseCamRotationOffset)
	{
		if (YawValue >= CameraAngles[3]) {
			AddActorWorldOffset(FVector(+CamRotationOffset, +CamRotationOffset, 0));
		}
		else if (YawValue >= CameraAngles[2]) {
			AddActorWorldOffset(FVector(+CamRotationOffset, -CamRotationOffset, 0));
		}
		else if (YawValue >= CameraAngles[1]) {
			AddActorWorldOffset(FVector(-CamRotationOffset, -CamRotationOffset, 0));
		}
		else if (YawValue >= CameraAngles[0]) {
			AddActorWorldOffset(FVector(-CamRotationOffset, +CamRotationOffset, 0));
		}
	}
	if (YawValue == -1) YawValue = 359.f;
	if (YawValue == CameraAngles[0] || YawValue == CameraAngles[3] || YawValue == CameraAngles[2] || YawValue == CameraAngles[1]) return true;
	
	return false;
}

bool ACameraBase::RotateCamLeftTo(float Position)
{
	if (abs(YawValue - Position) <= 1.f) return true;
	
	YawValue += 1.f;
	if (YawValue == 360) YawValue = 0.f;
	if (YawValue > CameraAngles[3]+RotationDegreeStep) YawValue -= CameraAngles[3]+RotationDegreeStep;
	
	return false;
}

bool ACameraBase::RotateCamRightTo(float Position)
{
	
	if (abs(YawValue - Position) <= 1.f) return true;
	
	YawValue -= 1.f;
	if (YawValue == -1) YawValue = 359.f;
	if (YawValue < CameraAngles[0]) YawValue += CameraAngles[3]+RotationDegreeStep;
	
	return false;
}

FVector2D ACameraBase::GetMousePos2D()
{
	float PosX;
	float PosY;
	PC->GetMousePosition(PosX, PosY);

	return FVector2D(PosX, PosY);
}


void ACameraBase::Zoom()
{
	FHitResult Hit;
	FVector2D MPosition = GetMousePos2D();
	FVector ActorLocation = GetActorLocation();
	SetActorLocation(FVector(MPosition.X, MPosition.Y, ActorLocation.Z));
}

void ACameraBase::ZoomOutToPosition(float Distance, const FVector SelectedActorPosition)
{
	
	FVector ALocation = GetActorLocation();
	FVector Difference = ALocation-SelectedActorPosition;
	
	const float k = (Difference.Z)/sqrt(Difference.X*Difference.X+Difference.Y*Difference.Y);

	float DiffX = (ALocation.X - SelectedActorPosition.X);
	float DiffY = (ALocation.Y - SelectedActorPosition.Y);
	float DiffXY = sqrt(DiffX*DiffX + DiffY*DiffY);

	
	if (DiffXY < Distance)
	{
		const float CosYaw = FMath::Cos(YawValue*PI/180);
		const float SinYaw = FMath::Sin(YawValue*PI/180);
		
		const FVector NewPawnLocation = FVector(4.25*CosYaw*(-1),  4.25*SinYaw*(-1), 0);
		ZoomXYDistance-=100;

		AddActorWorldOffset(NewPawnLocation * CamSpeed);

	}
}


bool ACameraBase::ZoomInToPosition(float Distance, const FVector SelectedActorPosition)
{
	const FVector ALocation = GetActorLocation();
	FVector Difference = ALocation-SelectedActorPosition;
	const float k = (Difference.Z)/sqrt(Difference.X*Difference.X+Difference.Y*Difference.Y);

	float DiffX = (ALocation.X - SelectedActorPosition.X);
	float DiffY = (ALocation.Y - SelectedActorPosition.Y);
	float DiffXY = sqrt(DiffX*DiffX + DiffY*DiffY);
	

	if (DiffXY > Distance)
	{
		const float CosYaw = FMath::Cos(YawValue*PI/180);
		const float SinYaw = FMath::Sin(YawValue*PI/180);

		const FVector NewPawnLocation = FVector(4.25*CosYaw, 4.25*SinYaw, 0);
		ZoomXYDistance+=100;

		AddActorWorldOffset(NewPawnLocation * ZoomSpeed);
		
		return false;
	}
	return true;
}


void ACameraBase::LockOnUnit(AUnitBase* Unit)
{
	if (Unit) {

		const FVector SelectedActorLocation = Unit->GetActorLocation();
		const FVector CurrentCameraPawnLocation = GetActorLocation();

		FVector Velo = Unit->GetVelocity();
		/*
		if(abs(Velo.Z) == 0.f)
		{
			LastActorZeroPosition = SelectedActorLocation.Z;
		}else if(Velo.Z > 100.f)
		{
			DeltaDecrementer = (SelectedActorLocation.Z - LastActorZeroPosition)/150.f;
		}
		else if(Velo.Z < 100.f)
		{
			LastActorZeroPosition += DeltaDecrementer;
			LastActorZeroPosition >= SelectedActorLocation.Z? LastActorZeroPosition = SelectedActorLocation.Z : LastActorZeroPosition;
		}*/
		
		float DeltaZ =  abs(SelectedActorLocation.Z - LastActorZeroPosition)*DeltaZScaler;
		
		const float CosYaw = FMath::Cos(YawValue*PI/180);
		const float SinYaw = FMath::Sin(YawValue*PI/180);
		const FVector NewPawnLocation = FVector(SelectedActorLocation.X+ZoomXYDistance*CosYaw-DeltaZ* 0.7*CosYaw, SelectedActorLocation.Y+ZoomXYDistance*SinYaw - DeltaZ*0.7*SinYaw,  SelectedActorLocation.Z+CamZOffset); // CurrentCameraPawnLocation.Z+CamZOffset
		//const FVector NewPawnLocation =  FVector(SelectedActorLocation.X, SelectedActorLocation.Y,  SelectedActorLocation.Z);
		SetActorLocation(SelectedActorLocation);
	
	}
}

bool ACameraBase::IsCharacterDistanceTooLow(float Distance, const FVector SelectedActorPosition)
{
	FVector ALocation = GetActorLocation();

	float DiffX = (ALocation.X - SelectedActorPosition.X);
	float DiffY = (ALocation.Y - SelectedActorPosition.Y);
	float DiffXY = sqrt(DiffX*DiffX + DiffY*DiffY);
	
	if(DiffXY < Distance)
	{
		return true;
	}
	return false;
}

void ACameraBase::HideControlWidget()
{
	if (ControlWidgetComp)
		ControlWidgetComp->SetRelativeLocation(ControlWidgetHideLocation);
}

void ACameraBase::ShowControlWidget()
{
	if (ControlWidgetComp)
		ControlWidgetComp->SetRelativeLocation(ControlWidgetLocation);
}

void ACameraBase::JumpCamera(FHitResult Hit)
{
	FVector ActorLocation = GetActorLocation();

	const float CosYaw = FMath::Cos(YawValue*PI/180);
	const float SinYaw = FMath::Sin(YawValue*PI/180);
	const FVector NewPawnLocation = FVector(Hit.Location.X - ActorLocation.Z * 0.7*CosYaw,  Hit.Location.Y - ActorLocation.Z * 0.7*SinYaw, ActorLocation.Z);

	SetActorLocation(NewPawnLocation);
}


void ACameraBase::MoveCamToForward()
{
	const float CosYaw = FMath::Cos(YawValue*PI/180);
	const float SinYaw = FMath::Sin(YawValue*PI/180);
	
	const FVector NewPawnLocation = FVector(0.3*CosYaw,  0.3*SinYaw, 0);
	
	AddActorWorldOffset(NewPawnLocation * CamSpeed);
}

void ACameraBase::MoveCamToBackward()
{
	const float CosYaw = FMath::Cos(YawValue*PI/180);
	const float SinYaw = FMath::Sin(YawValue*PI/180);
	
	const FVector NewPawnLocation = FVector(0.3*CosYaw*(-1),  0.3*SinYaw*(-1), 0);
	
	AddActorWorldOffset(NewPawnLocation * CamSpeed);
}

void ACameraBase::MoveCamToLeft()
{

	const float CosYaw = FMath::Cos(YawValue*PI/180);
	const float SinYaw = FMath::Sin(YawValue*PI/180);
	
	const FVector NewPawnLocation = FVector(0.3*SinYaw,  0.3*CosYaw*(-1), 0);
	
	AddActorWorldOffset(NewPawnLocation * CamSpeed);
}

void ACameraBase::MoveCamToRight()
{
	const float CosYaw = FMath::Cos(YawValue*PI/180);
	const float SinYaw = FMath::Sin(YawValue*PI/180);
	
	const FVector NewPawnLocation = FVector(0.3*SinYaw*(-1),  0.3*CosYaw, 0);
	
	AddActorWorldOffset(NewPawnLocation * CamSpeed);
}


void ACameraBase::SetUserWidget(AExtendedUnitBase* SelectedActor)
{
	UUnitBaseActionBar* ActionBar = Cast<UUnitBaseActionBar>(ActionWidgetComp->GetUserWidgetObject());
	
	if (ActionBar) {
		ActionBar->SetOwnerActor(SelectedActor);
	}
}

void ACameraBase::ChangeWeaponIndicator(USkeletalMesh* NewWeaponMesh)
{
	UIWeaponIndicator->ChangeWeaponIndicator(NewWeaponMesh);
}


void ACameraBase::SpawnActionWidget()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpellTransform;
	SpellTransform.SetLocation(FVector(500, 0, 0));
	SpellTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	if (ActionWidgetComp) {
		FRotator NewRotation = ActionWidgetRotation;
		FQuat QuatRotation = FQuat(NewRotation);
		ActionWidgetComp->SetRelativeRotation(QuatRotation, false, 0, ETeleportType::None);
		ActionWidgetComp->SetRelativeLocation(ActionWidgetLocation);
	}

}
void ACameraBase::SpawnWeaponIndicator()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform SpellTransform;
	SpellTransform.SetLocation(WeaponIndictatorLocation);
	SpellTransform.SetRotation(FQuat(FRotator::ZeroRotator));

	UIWeaponIndicator = GetWorld()->SpawnActor<AUIWeaponIndicator>(AUIWeaponIndicator::StaticClass(), SpellTransform, SpawnParams);

	if (UIWeaponIndicator) {
		UIWeaponIndicator->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		UIWeaponIndicator->SetActorLocation(GetActorLocation() + WeaponIndictatorLocation);
		UIWeaponIndicator->SetActorScale3D(FVector(0.5f));
	}
}

void ACameraBase::SetCameraState(TEnumAsByte<CameraData::CameraState> NewCameraState)
{
	CameraState = NewCameraState;
}

TEnumAsByte<CameraData::CameraState> ACameraBase::GetCameraState()
{
	return CameraState;
}
/*
void ACameraBase::SwitchControllerStateMachine(const FInputActionValue& InputActionValue, int32 NewCameraState)
{
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	
	if(CameraControllerBase)
	{
		if(!CameraControllerBase->IsStrgPressed)
		switch (NewCameraState)
		{
		case 3:
			{
				CameraControllerBase->APressed();
			}break;
		case 7:
			{
				CameraControllerBase->ZoomInToPosition = false;
				CameraControllerBase->ZoomOutToPosition = true;
			} break;
		case 8:
			{
				CameraControllerBase->ZoomInToPosition = true;
				CameraControllerBase->ZoomOutToPosition = false;
			} break;
		case 9:
			{
				CameraControllerBase->CamIsRotatingLeft = false;
				CameraControllerBase->CamIsRotatingRight = true;
			} break;
		case 10:
			{

				CameraControllerBase->CamIsRotatingRight = false;
				CameraControllerBase->CamIsRotatingLeft = true;
			} break;
		case 19:
			{
				// Q Released
				//CameraControllerBase->QReleased();
			} break;
		case 13:
			{
				// R Pressed
				CameraControllerBase->RPressed();
			} break;
		case 14:
			{
				// F Pressed
				// CameraControllerBase->FPressed();
			} break;
		case 15:
			{
				// C Pressed
				// CameraControllerBase->CPressed();
			} break;
		case 16:
			{
				// S Pressed
					CameraControllerBase->SReleased();
			} break;
		case 20:
			{
				// Joystick 1 X 
				float IValue = InputActionValue.Get<float>();
				if (YawValue == CameraAngles[0] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue, 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2, 0.f, IValue));
				}else if (YawValue == CameraAngles[2] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue*(-1), 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2*(-1), 0.f, IValue));
				}else if (YawValue == CameraAngles[1] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue*(-1)));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2*(-1), IValue));
				}else if (YawValue == CameraAngles[3] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2, IValue));
				}
				
					
				
			} break;
		case 21:
			{
				// Joystick 1 Y
				
				float IValue = InputActionValue.Get<float>();
				if (YawValue == CameraAngles[1] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue, 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2, 0.f, IValue));
				}else if (YawValue == CameraAngles[3] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue*(-1), 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2*(-1), 0.f, IValue));
				}else if (YawValue == CameraAngles[0] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2, IValue));
				}else if (YawValue == CameraAngles[2] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue*(-1)));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2*(-1), IValue));
				}
				
					
				
			} break;
		case 22:
			{
				// Gamepad A
				CameraControllerBase->APressed();
				
			} break;
		case 23:
			{
				// Gamepad A
				CameraControllerBase->AReleased();
				
			} break;
		case 24:
			{
				// Gamepad B
			} break;
		case 25:
			{
				// Gamepad B
				CameraControllerBase->EPressed();
			} break;
		case 26:
			{
				// Gamepad X
				CameraControllerBase->CPressed();
			} break;
		case 27:
			{
				// Gamepad X
			} break;
		case 28:
			{
				UE_LOG(LogTemp, Warning, TEXT("Gamepad Y Pressed"));

				for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilityFive for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilityFive, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
				CameraControllerBase->QPressed();
			} break;
		case 29:
			{
				UE_LOG(LogTemp, Warning, TEXT("Gamepad Y Released"));
				CameraControllerBase->QReleased();
				
			} break;
		case 30:
			{
				// Joystick 2 X
				UE_LOG(LogTemp, Warning, TEXT("Joystick 2 X"));
				float FValue = InputActionValue.Get<float>();

				if(FValue >= 1)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = true;
				}
				else if(FValue <= -1)
				{
					CameraControllerBase->CamIsRotatingRight = false;
					CameraControllerBase->CamIsRotatingLeft = true;
				}
			} break;
		case 31:
			{
				//  Joystick 2 Y
				float FValue = InputActionValue.Get<float>();
				
				if(FValue >= 1)
				{
					CameraControllerBase->ZoomInToPosition = false;
					CameraControllerBase->ZoomOutToPosition = true;
				}else{
					CameraControllerBase->ZoomInToPosition = true;
					CameraControllerBase->ZoomOutToPosition = false;
				}


			} break;
		case 32:
			{
				//  L Shoulder
				CameraControllerBase->SetAxis();
				SwitchAxis = !SwitchAxis;
				CameraControllerBase->CamIsRotatingRight = false;
				CameraControllerBase->CamIsRotatingLeft = true;
	
			} break;
		case 33:
			{
				//  R Shoulder
				CameraControllerBase->SetAxis();
				SwitchAxis = !SwitchAxis;
				CameraControllerBase->CamIsRotatingLeft = false;
				CameraControllerBase->CamIsRotatingRight = true;
			} break;
		case 34:
			{
				// UE_LOG(LogTemp, Warning, TEXT("L Trigger Pressed"));
				CameraControllerBase->SetDropJumpMine(true);
			} break;
		case 35:
			{
				// UE_LOG(LogTemp, Warning, TEXT("L Trigger Released"));
				CameraControllerBase->SetDropJumpMine(false);
			} break;
		case 36:
			{
				ShowControlWidget();
			} break;
		case 37:
			{
				HideControlWidget();
			} break;
		case 38:
			{
				float IValue = -1.0f;

				if (YawValue == CameraAngles[0] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue, 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2, 0.f, IValue));
				}else if (YawValue == CameraAngles[2] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue*(-1), 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2*(-1), 0.f, IValue));
				}else if (YawValue == CameraAngles[1] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue*(-1)));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2*(-1), IValue));
				}else if (YawValue == CameraAngles[3] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2, IValue));
				}
			} break;
		case 39:
			{
				float IValue = 1.0f;
				
				if (YawValue == CameraAngles[0] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue, 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2, 0.f, IValue));
				}else if (YawValue == CameraAngles[2] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue*(-1), 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2*(-1), 0.f, IValue));
				}else if (YawValue == CameraAngles[1] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue*(-1)));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2*(-1), IValue));
				}else if (YawValue == CameraAngles[3] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2, IValue));
				}
				
			} break;
		case 40:
			{
				float IValue = 1.0f;
				if (YawValue == CameraAngles[1] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue, 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2, 0.f, IValue));
				}else if (YawValue == CameraAngles[3] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue*(-1), 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2*(-1), 0.f, IValue));
				}else if (YawValue == CameraAngles[0] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2, IValue));
				}else if (YawValue == CameraAngles[2] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue*(-1)));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2*(-1), IValue));
				}
				
			} break;
		case 41:
			{
				float IValue = -1.0f;
				if (YawValue == CameraAngles[1] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue, 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2, 0.f, IValue));
				}else if (YawValue == CameraAngles[3] && !SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(IValue*(-1), 0.f));
					CameraControllerBase->TripleJump(FVector(IValue*2*(-1), 0.f, IValue));
				}else if (YawValue == CameraAngles[0] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2, IValue));
				}else if (YawValue == CameraAngles[2] && SwitchAxis) {
					CameraControllerBase->JoystickRunUnit(FVector2D(0.f, IValue*(-1)));
					CameraControllerBase->TripleJump(FVector(0.f, IValue*2*(-1), IValue));
				}
				
			} break;
		}
		
	}

}
*/