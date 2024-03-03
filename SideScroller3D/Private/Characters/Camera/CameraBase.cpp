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

// Called when the game starts or when spawned
void ACameraBase::BeginPlay()
{
	Super::BeginPlay();
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

	SpringArm->TargetArmLength = DefaultSpringArmLength;
	//SpawnWeaponIndicator();
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


void ACameraBase::PanMoveCamera(const FVector& NewPanDirection) {
	if (NewPanDirection != FVector::ZeroVector) {
		AddActorWorldOffset(NewPanDirection * GetActorLocation().Z * 0.001);
	}
}

void ACameraBase::ZoomOut() {
	if(SpringArm)
		SpringArm->TargetArmLength += CamSpeed;
	//AddActorWorldOffset(FVector(0, 0, 0.3) * CamSpeed);
}

void ACameraBase::ZoomIn() {

	if(SpringArm)
		SpringArm->TargetArmLength -= CamSpeed;
	//if(GetActorLocation().Z > 0.f)
		//AddActorWorldOffset(FVector(0, 0, -0.3) * CamSpeed);
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

		FVector Velo = Unit->GetVelocity();
		
		//UE_LOG(LogTemp, Warning, TEXT("Velo.Z: %f"), Velo.Z);
		if(Velo.Z == 0.f) SpringArm->TargetArmLength = DefaultSpringArmLength;
		else
		{
			float NewSpringArmLength = SpringArm->TargetArmLength += Velo.Z*0.005;
			if(NewSpringArmLength <= DefaultSpringArmLength) SpringArm->TargetArmLength = DefaultSpringArmLength;
			else  SpringArm->TargetArmLength = NewSpringArmLength;
		}
		
		const FVector NewPawnLocation =  FVector(SelectedActorLocation.X, SelectedActorLocation.Y,  SelectedActorLocation.Z+CamZOffset);
		SetActorLocation(NewPawnLocation);
	
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
		ControlWidgetComp->SetVisibility(false);
}

void ACameraBase::ShowControlWidget()
{
	if (ControlWidgetComp)
		ControlWidgetComp->SetVisibility(true);
}

void ACameraBase::JumpCamera(FHitResult Hit)
{
	FVector ActorLocation = GetActorLocation();

	const float CosYaw = FMath::Cos(YawValue*PI/180);
	const float SinYaw = FMath::Sin(YawValue*PI/180);
	const FVector NewPawnLocation = FVector(Hit.Location.X - ActorLocation.Z * 0.7*CosYaw,  Hit.Location.Y - ActorLocation.Z * 0.7*SinYaw, ActorLocation.Z);

	SetActorLocation(NewPawnLocation);
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
	if(UIWeaponIndicator)
	UIWeaponIndicator->ChangeWeaponIndicator(NewWeaponMesh);
}

/*

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
*/
void ACameraBase::SetCameraState(TEnumAsByte<CameraData::CameraState> NewCameraState)
{
	CameraState = NewCameraState;
}

TEnumAsByte<CameraData::CameraState> ACameraBase::GetCameraState()
{
	return CameraState;
}
