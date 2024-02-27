
#include "Characters/Camera/ExtendedCameraBase.h"

#include "Controller/CameraControllerBase.h"
#include "Controller/Input/GameplayTags.h"
#include "GAS/GAS.h"
#include "Widgets/AbilityChooser.h"

#include "Widgets/TalentChooser.h"


AExtendedCameraBase::AExtendedCameraBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (RootComponent == nullptr) {
		RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("Root"));
	}
	
	CreateCameraComp();

	ControlWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("ControlWidget"));
	ControlWidgetComp->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	TalentChooser = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("TalentChooser"));
	TalentChooser->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);
	
	AbilityChooser = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("AbilityChooser"));
	AbilityChooser->AttachToComponent(RootScene, FAttachmentTransformRules::KeepRelativeTransform);

	ActionWidgetComp = ObjectInitializer.CreateDefaultSubobject<UWidgetComponent>(this, TEXT("ActionWidgetComp"));
	ActionWidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	

	
		//GetMesh()->AttachToComponent(GetCameraBaseCapsule(), FAttachmentTransformRules::KeepRelativeTransform);


		UCapsuleComponent* CComponent = GetCapsuleComponent();
		if (CComponent)
		{
			CComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);  // Enable both physics and overlap query
			CComponent->SetCollisionResponseToAllChannels(ECR_Ignore);  // Start by ignoring all channels
			CComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);  // Block other pawns (this can be adjusted based on your requirements)
			CComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);  // Important: Block WorldStatic so it can walk on static objects like ground, walls, etc.
			CComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);  // ECR_Overlap Overlap with dynamic objects (adjust as needed)
		}

	
		UMeshComponent* CMesh = GetMesh();
		if(CMesh)
		{
	
			CMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);  // Typically, we use the capsule for physics and mesh for simple queries like overlap
			CMesh->SetCollisionResponseToAllChannels(ECR_Ignore);  // Start by ignoring all channels
			CMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);  // Overlap with other pawns
			CMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);  // Overlap with dynamic objects
		}

}

// BeginPlay implementation
void AExtendedCameraBase::BeginPlay()
{
	// Call the base class BeginPlay
	UE_LOG(LogTemp, Warning, TEXT("ExtendedCameraBase BEGINPLAY!!!!! "));
	Super::BeginPlay();

}

// Tick implementation
void AExtendedCameraBase::Tick(float DeltaTime)
{
	// Call the base class Tick
	Super::Tick(DeltaTime);
}


bool AExtendedCameraBase::IsOwnedByLocalPlayer()
{
	APlayerController* MyController = Cast<APlayerController>(GetController());
	return MyController && MyController->IsLocalPlayerController();
}

void AExtendedCameraBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponentBase* EnhancedInputComponentBase = Cast<UEnhancedInputComponentBase>(PlayerInputComponent);

	if(EnhancedInputComponentBase)
	{
		check(EnhancedInputComponentBase);
		const FGameplayTags& GameplayTags = FGameplayTags::Get();
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Space_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 7);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Space_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 8);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Alt_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 9);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Ctrl_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 10);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_S_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 2);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_S_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 0);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_R_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 13);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_F_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 14);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_C_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 15);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_S_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 16);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Q_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 19);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Joystick_1_X, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 20);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Joystick_1_Y, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 21);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_A_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 22);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_A_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 23);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_B_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 24);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_B_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 25);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_X_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 26);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_X_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 27);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_Y_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 28);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Gamepad_Y_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 29);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Joystick_2_X, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 30);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Joystick_2_Y, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 31);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Shoulder, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 32);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Right_Shoulder, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 33);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Trigger_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 34);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Trigger_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 35);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Tab_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 36);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Tab_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 37);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Left, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 38);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Right, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 39);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Up, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 40);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Down, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 41);
	}
}

void AExtendedCameraBase::SetExtendedUserWidget(AUnitBase* SelectedActor)
{

	UTalentChooser* TalentBar= Cast<UTalentChooser>(TalentChooser->GetUserWidgetObject());
	UAbilityChooser* AbilityBar= Cast<UAbilityChooser>(AbilityChooser->GetUserWidgetObject());
	
	if(!TalentBar) return;

	if(SelectedActor)
	{
		if (TalentBar) {
			TalentBar->SetVisibility(ESlateVisibility::Visible);
			TalentBar->SetOwnerActor(SelectedActor);
			TalentBar->CreateClassUIElements();
		}

		if (AbilityBar) {
			AbilityBar->SetVisibility(ESlateVisibility::Visible);
			AbilityBar->SetOwnerActor(SelectedActor);
		}
		
	}else
	{
		TalentBar->SetVisibility(ESlateVisibility::Collapsed);
		AbilityBar->SetVisibility(ESlateVisibility::Collapsed);
	}

}



void AExtendedCameraBase::OnAbilityInputDetected(EGASAbilityInputID InputID, AGASUnit* SelectedUnit, const TArray<TSubclassOf<UGameplayAbilityBase>>& AbilitiesArray)
{
	if(SelectedUnit)
	{
		//UE_LOG(LogTemp, Warning, TEXT("OnAbilityInputDetected: Activating ability ID %d for unit: %s"), static_cast<int32>(InputID), *SelectedUnit->GetName());
		SelectedUnit->ActivateAbilityByInputID(InputID, AbilitiesArray);
	}
}

void AExtendedCameraBase::Input_LeftClick_Pressed(const FInputActionValue& InputActionValue, int32 Camstate)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	if(CameraControllerBase)
	{
		CameraControllerBase->LeftClickPressed();
		CameraControllerBase->JumpCamera();
	}

	if(CameraControllerBase && CameraControllerBase->IsShiftPressed)
	{
		SetCameraState(CameraData::MoveToClick);
	}
}

void AExtendedCameraBase::Input_LeftClick_Released(const FInputActionValue& InputActionValue, int32 Camstate)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	if(CameraControllerBase)
	{
		CameraControllerBase->LeftClickReleased();
	}
}

void AExtendedCameraBase::Input_RightClick_Pressed(const FInputActionValue& InputActionValue, int32 Camstate)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	if(CameraControllerBase)
	{
		CameraControllerBase->RightClickPressed();
	}
}

void AExtendedCameraBase::Input_G_Pressed(const FInputActionValue& InputActionValue, int32 Camstate)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	if(CameraControllerBase)
	{
		CameraControllerBase->ToggleLockCamToCharacter();
	}
}

void AExtendedCameraBase::Input_A_Pressed(const FInputActionValue& InputActionValue, int32 CamState)
{
	if(BlockControls) return;

	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	if(CameraControllerBase)
	{
		//CameraControllerBase->TPressed();
	}
}

void AExtendedCameraBase::Input_Ctrl_Pressed(const FInputActionValue& InputActionValue, int32 Camstate)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	if(CameraControllerBase)
	{
		CameraControllerBase->IsStrgPressed = true;
	}
}

void AExtendedCameraBase::Input_Ctrl_Released(const FInputActionValue& InputActionValue, int32 Camstate)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	if(CameraControllerBase)
	{
		CameraControllerBase->IsStrgPressed = false;
	}
}

void AExtendedCameraBase::Input_Tab_Pressed(const FInputActionValue& InputActionValue, int32 CamState)
{
	if(BlockControls) return;
	
	ShowControlWidget();
}

void AExtendedCameraBase::Input_Tab_Released(const FInputActionValue& InputActionValue, int32 CamState)
{
	if(BlockControls) return;
	
	HideControlWidget();
}

void AExtendedCameraBase::Input_Shift_Pressed(const FInputActionValue& InputActionValue, int32 CamState)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	
	if(CameraControllerBase)
	{
		CameraControllerBase->ShiftPressed();
	}
}

void AExtendedCameraBase::Input_Shift_Released(const FInputActionValue& InputActionValue, int32 CamState)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	
	if(CameraControllerBase)
	{
		CameraControllerBase->ShiftReleased();
	}
}
/*
void AExtendedCameraBase::SwitchControllerStateMachine(const FInputActionValue& InputActionValue, int32 NewCameraState)
{
	if(BlockControls) return;
	
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	
	if(CameraControllerBase)
	{
		if(CameraControllerBase->IsStrgPressed)
			switch (NewCameraState)
			{
		case 0:
			{

			}break;
		case 1:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->WIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 111:
			{
				CameraControllerBase->WIsPressedState = 2;
			} break;
		case 2:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->SIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 222:
			{
				CameraControllerBase->SIsPressedState = 2;
			} break;
		case 3:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->AIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 333:
			{
				CameraControllerBase->AIsPressedState = 2;
			} break;
		case 4:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->DIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 444:
			{
				CameraControllerBase->DIsPressedState = 2;
			} break;
		case 5:
			{
				if(CameraControllerBase->LockCameraToCharacter)
				{
					if(!CameraControllerBase->HoldZoomOnLockedCharacter) CameraControllerBase->CamIsZoomingInState = 1;
				
					CameraControllerBase->HoldZoomOnLockedCharacter = !CameraControllerBase->HoldZoomOnLockedCharacter;
				
				} else
				{
					CameraControllerBase->CamIsZoomingInState = 1;
					SetCameraState(CameraData::ZoomIn);
				}
			}
				break;
		case 555:
			{
					CameraControllerBase->CamIsZoomingInState = 2;
			}
			break;
		case 6:
			{
				if(CameraControllerBase->LockCameraToCharacter)
				{
					if(!CameraControllerBase->HoldZoomOnLockedCharacter) CameraControllerBase->CamIsZoomingOutState = 1;
				
					CameraControllerBase->HoldZoomOnLockedCharacter = !CameraControllerBase->HoldZoomOnLockedCharacter;
				
				} else
				{
					CameraControllerBase->CamIsZoomingOutState = 1;
					SetCameraState(CameraData::ZoomOut);
				}
			}
			break;
		case 666:
			{
				CameraControllerBase->CamIsZoomingOutState = 2;
			}
			break;
		case 7: SetCameraState(CameraData::ZoomOutPosition); break;
		case 8: SetCameraState(CameraData::ZoomInPosition); break;
		case 9:
			{
				if(CameraControllerBase->LockCameraToCharacter)
					return;
				
				SetCameraState(CameraData::RotateLeft);
			} break;
		case 10:
			{
				if(CameraControllerBase->LockCameraToCharacter)
					return;
				
				SetCameraState(CameraData::RotateRight);
			} break;
		case 11: SetCameraState(CameraData::LockOnCharacter); break;
		case 12: SetCameraState(CameraData::ZoomToThirdPerson); break;
		case 14:
			{	SetCameraState(CameraData::OrbitAndMove); break;
				//CameraControllerBase->OrbitAtLocation(FVector(1000.f, -1000.f, 500.f), 0.033f);
			} break;
		case 15:
			{

				CameraControllerBase->SpawnMissileRain(4, FVector(1000.f, -1000.f, 1000.f));
				CameraControllerBase->SpawnEffectArea(3, FVector(1000.f, -1000.f, 10.f), FVector(5), CameraControllerBase->EffectAreaClass);
			} break;
		case 16:
			{
				float MouseX, MouseY;
				CameraControllerBase->GetMousePosition(MouseX, MouseY);
				PreviousMouseLocation.X = MouseX;
				PreviousMouseLocation.Y = MouseY;

				CameraControllerBase->MiddleMouseIsPressed = true;
			} break;
		case 17:
			{
				CameraControllerBase->MiddleMouseIsPressed = false;
			} break;
		case 21:
			{
				
			} break;
		case 22:
			{
				
			} break;
		case 23:
			{
				
			} break;
		case 24:
			{
				
			} break;
		case 25:
			{
				
			} break;
		case 26:
			{
				
			} break;
		default:
			{
				//SetCameraState(CameraData::UseScreenEdges);
			}break;
		}

		if(!CameraControllerBase->IsStrgPressed)
		switch (NewCameraState)
		{
		case 1:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->WIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 111:
			{
				CameraControllerBase->WIsPressedState = 2;
			} break;
		case 2:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->SIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 222:
			{
				CameraControllerBase->SIsPressedState = 2;
			} break;
		case 3:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->AIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 333:
			{
				CameraControllerBase->AIsPressedState = 2;
			} break;
		case 4:
			{
				if(GetCameraState() == CameraData::OrbitAndMove)
				{
					CameraControllerBase->CamIsRotatingLeft = false;
					CameraControllerBase->CamIsRotatingRight = false;
				}
				
				CameraControllerBase->DIsPressedState = 1;
				CameraControllerBase->LockCameraToUnit = false;
				SetCameraState(CameraData::MoveWASD);
			} break;
		case 444:
			{
				CameraControllerBase->DIsPressedState = 2;
			} break;
		case 5:
			{
				if(CameraControllerBase->LockCameraToCharacter)
				{
					if(!CameraControllerBase->HoldZoomOnLockedCharacter) CameraControllerBase->CamIsZoomingInState = 1;
				
					CameraControllerBase->HoldZoomOnLockedCharacter = !CameraControllerBase->HoldZoomOnLockedCharacter;
				
				} else
				{
					CameraControllerBase->CamIsZoomingInState = 1;
					SetCameraState(CameraData::ZoomIn);
				}
			}
				break;
		case 555:
			{
					CameraControllerBase->CamIsZoomingInState = 2;
			}
			break;
		case 6:
			{
				if(CameraControllerBase->LockCameraToCharacter)
				{
					if(!CameraControllerBase->HoldZoomOnLockedCharacter) CameraControllerBase->CamIsZoomingOutState = 1;
				
					CameraControllerBase->HoldZoomOnLockedCharacter = !CameraControllerBase->HoldZoomOnLockedCharacter;
				
				} else
				{
					CameraControllerBase->CamIsZoomingOutState = 1;
					SetCameraState(CameraData::ZoomOut);
				}
			}
			break;
		case 666:
			{
				CameraControllerBase->CamIsZoomingOutState = 2;
			}
			break;
		case 8: SetCameraState(CameraData::ZoomInPosition); break;
		case 9:
			{
				CameraControllerBase->CamIsRotatingLeft = true;
				
				if(CameraControllerBase->LockCameraToCharacter ||
					CameraControllerBase->WIsPressedState ||
					CameraControllerBase->AIsPressedState ||
					CameraControllerBase->SIsPressedState ||
					CameraControllerBase->DIsPressedState)
					return;
				
				SetCameraState(CameraData::HoldRotateLeft);
			} break;
		case 10:
			{
				CameraControllerBase->CamIsRotatingRight = true;
				
				if(CameraControllerBase->LockCameraToCharacter ||
					CameraControllerBase->WIsPressedState ||
					CameraControllerBase->AIsPressedState ||
					CameraControllerBase->SIsPressedState ||
					CameraControllerBase->DIsPressedState)
					return;
				
				SetCameraState(CameraData::HoldRotateRight);
			} break;
		case 999:
			{
				CameraControllerBase->CamIsRotatingLeft = false;
			}
			break;
		case 101010:
			{
				CameraControllerBase->CamIsRotatingRight = false;
			}
			break;
		case 12:
			{
				CameraControllerBase->TPressed();
			}
			break;
		case 13:
			{
				float FloatValue = InputActionValue.Get<float>();

				if(CameraControllerBase->ScrollZoomCount <= 10.f)
				CameraControllerBase->ScrollZoomCount += FloatValue*2;
				
				if(CameraControllerBase->LockCameraToCharacter)
					return;
				
				if(FloatValue > 0)
				{
					SetCameraState(CameraData::ScrollZoomIn);
				}
				else
				{
					SetCameraState(CameraData::ScrollZoomOut);
				}
			}
			break;
		case 15:
			{
				UE_LOG(LogTemp, Warning, TEXT("15 pressed!"));

			} break;
		case 16:
			{
				float MouseX, MouseY;
				CameraControllerBase->GetMousePosition(MouseX, MouseY);
				PreviousMouseLocation.X = MouseX;
				PreviousMouseLocation.Y = MouseY;
				
				CameraControllerBase->MiddleMouseIsPressed = true;
			} break;
		case 17:
			{
				CameraControllerBase->MiddleMouseIsPressed = false;
			} break;
		case 21:
			{
				for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilityOne for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilityOne, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
			} break;
		case 22:
			{
				for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilityTwo for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilityTwo, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
			} break;
		case 23:
			{
				for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilityThree for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilityThree, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
			} break;
		case 24:
			{
				for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilityFour for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilityFour, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
			} break;
		case 25:
			{
				for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilityFive for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilityFive, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
			} break;
		case 26:
			{
				for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilitySix for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilitySix, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
			} break;
		}
	}

}
*/


void AExtendedCameraBase::SwitchControllerStateMachine(const FInputActionValue& InputActionValue, int32 NewCameraState)
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
						UE_LOG(LogTemp, Warning, TEXT("Activating AbilityOne for unit: %s"), *SelectedUnit->GetName());
						OnAbilityInputDetected(EGASAbilityInputID::AbilityOne, SelectedUnit, SelectedUnit->DefaultAbilities);
					}
				}
				//CameraControllerBase->QPressed();
			} break;
		case 29:
			{
				UE_LOG(LogTemp, Warning, TEXT("Gamepad Y Released"));
				//CameraControllerBase->QReleased();
				
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
