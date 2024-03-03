
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

		/*
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Q_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 22); 
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Q_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 23);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_W_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 24);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_W_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 25);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_E_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 26);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_E_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 27);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_A_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 28);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_A_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 29);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_S_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 32);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_S_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 33);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_D_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 34);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_D_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 35);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Space_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 36);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Space_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 37);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Ctrl_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 36);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Ctrl_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 37);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Alt_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 38);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Alt_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 39);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_R_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 48);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_T_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 49);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_F_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 22);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_C_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 24);
		*/
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_S_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 0);
		
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

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Shoulder_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 34);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Right_Shoulder_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 35);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Shoulder_2, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 36);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Right_Shoulder_2, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 37);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Shoulder_2_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 38);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Right_Shoulder_2_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 39);
		
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Trigger_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 40);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Left_Trigger_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 41);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Tab_Pressed, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 42);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Tab_Released, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 43);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Left, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 44);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Right, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 45);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Left, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 46);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Right, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 47);

		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Up, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 48);
		EnhancedInputComponentBase->BindActionByTag(InputConfig, GameplayTags.InputTag_Arrow_Down, ETriggerEvent::Triggered, this, &AExtendedCameraBase::SwitchControllerStateMachine, 49);
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

void AExtendedCameraBase::FindUButtonWithMouseHover()
{

	UTalentChooser* TalentBar = Cast<UTalentChooser>(TalentChooser->GetUserWidgetObject());
	if (TalentBar)
	{
		UButton* HitButton = TalentBar->GetButtonUnderCursor();
		if (HitButton)
		{
			FocusedButton = HitButton;
			// Do something with the HitButton
		}
	}

	UAbilityChooser* AbilityBar = Cast<UAbilityChooser>(AbilityChooser->GetUserWidgetObject());
	if (AbilityBar)
	{
		UButton* HitButton = AbilityBar->GetButtonUnderCursor();
		if (HitButton)
		{
			FocusedButton = HitButton;
			// Do something with the HitButton
		}
	}

}

void AExtendedCameraBase::OnSimulatedMouseClick()
{
	if (FocusedButton)
	{
		// Simulate a click on the focused button
		FocusedButton->OnClicked.Broadcast();
	}
}

void AExtendedCameraBase::JoyStickMoveCamDependent(ACameraControllerBase* CameraControllerBase, float Value, int Angles[4])
{
	if (YawValue == CameraAngles[Angles[0]]) { //  && !SwitchAxis
		CameraControllerBase->JoystickRunUnit(FVector2D(Value, 0.f));
	}else if (YawValue == CameraAngles[Angles[1]]) { //  && !SwitchAxis
		CameraControllerBase->JoystickRunUnit(FVector2D(Value*(-1), 0.f));
	}else if (YawValue == CameraAngles[Angles[2]]) { //  && SwitchAxis
		CameraControllerBase->JoystickRunUnit(FVector2D(0.f, Value*(-1)));
	}else if (YawValue == CameraAngles[Angles[3]]) { //  && SwitchAxis
		CameraControllerBase->JoystickRunUnit(FVector2D(0.f, Value));
	}
}

void AExtendedCameraBase::SwitchControllerStateMachine(const FInputActionValue& InputActionValue, int32 NewCameraState)
{
	ACameraControllerBase* CameraControllerBase = Cast<ACameraControllerBase>(GetController());
	
	if(CameraControllerBase)
	{
		if(!CameraControllerBase->IsStrgPressed)
		switch (NewCameraState)
		{
		case 0:
			{
				// Switch Weapon // Joystick 1 Presed
				CameraControllerBase->SReleased();
			} break;
		case 20:
			{
				// Joystick 1 X 
				float IValue = InputActionValue.Get<float>();
				JoyStickX = IValue;
				
				if(CameraControllerBase->LShoulder2Pressed)
				{
					FVector2D MousePosition;
					CameraControllerBase->GetMousePosition(MousePosition.X, MousePosition.Y);
					MousePosition.X += IValue*4; // Modify this formula as needed
					CameraControllerBase->SetMouseLocation(MousePosition.X, MousePosition.Y);
					FindUButtonWithMouseHover();
				}else
				{
					if(!SwitchAxis)
					{
						int Angles[4] = {0, 2, 1, 3};
						JoyStickMoveCamDependent(CameraControllerBase, IValue, Angles);
					}
				
					if(CameraControllerBase->YIsPressed)
					{
						for (AGASUnit* SelectedUnit : CameraControllerBase->SelectedUnits)
						{
							if (SelectedUnit)
							{
								//UE_LOG(LogTemp, Warning, TEXT("Activating AbilityTwo for unit: %s"), *SelectedUnit->GetName());
								OnAbilityInputDetected(EGASAbilityInputID::AbilityTwo, SelectedUnit, SelectedUnit->DefaultAbilities);
							}
						}
					}
				}
			} break;
		case 21:
			{
				// Joystick 1 Y
				
				float IValue = InputActionValue.Get<float>();
				JoyStickY = IValue;
				if(CameraControllerBase->LShoulder2Pressed)
				{
					FVector2D MousePosition;
					CameraControllerBase->GetMousePosition(MousePosition.X, MousePosition.Y);
					MousePosition.Y -= IValue*4; // Modify this formula as needed
					CameraControllerBase->SetMouseLocation(MousePosition.X, MousePosition.Y);
					FindUButtonWithMouseHover();
				}else
				{
					if(SwitchAxis)
					{
						int Angles[4] = {1, 3, 2, 0};
						JoyStickMoveCamDependent(CameraControllerBase, IValue, Angles);
					}
				}
			} break;
		case 22:
			{
				// Gamepad A Pressed
				//CameraControllerBase->APressed();
				//UE_LOG(LogTemp, Warning, TEXT("True!"));
				CameraControllerBase->AIsPressed = true;

				if(CameraControllerBase->RShoulderPressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Activating ThrowAbilityID for unit: %s"), *SelectedUnit->GetName());
							OnAbilityInputDetected(SelectedUnit->ThrowAbilityID, SelectedUnit, SelectedUnit->ThrowAbilities);
						}
					}
				}else if(CameraControllerBase->LShoulderPressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							OnAbilityInputDetected(EGASAbilityInputID::AbilitySix, SelectedUnit, SelectedUnit->DefaultAbilities);
						}
					}
				}else if(CameraControllerBase->RShoulder2Pressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if(SelectedUnit)
						{
							AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnit);
							if (ExtendedUnitBase)
							{
								//UE_LOG(LogTemp, Warning, TEXT("TabNextUnitToChase!"));
								ExtendedUnitBase->TabNextUnitToChase();
								
							}
						}
					}
				}else if(!CameraControllerBase->LShoulder2Pressed && !CameraControllerBase->RShoulder2Pressed && !CameraControllerBase->BIsPressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Activating AbilityFour for unit: %s"), *SelectedUnit->GetName());
							OnAbilityInputDetected(EGASAbilityInputID::AbilityFour, SelectedUnit, SelectedUnit->DefaultAbilities);
						}
					}
				}
			} break;
		case 23:
			{
				// Gamepad A Released
				CameraControllerBase->AReleased();
				CameraControllerBase->AIsPressed = false;
			} break;
		case 24:
			{
				// Gamepad B Pressed
				CameraControllerBase->BIsPressed = true;

				if(CameraControllerBase->RShoulderPressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Activating AttackAbilityID for unit: %s"), *SelectedUnit->GetName());
							OnAbilityInputDetected(SelectedUnit->AttackAbilityID, SelectedUnit, SelectedUnit->AttackAbilities);
						}
					}
				}else if(CameraControllerBase->RShoulder2Pressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if(SelectedUnit)
						{
							AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(SelectedUnit);
							if (ExtendedUnitBase)
							{
								//UE_LOG(LogTemp, Warning, TEXT("TabPrevUnitToChase!"));
								ExtendedUnitBase->TabPrevUnitToChase();
							}
						}
					}
				}else
				{
					CameraControllerBase->EPressed();
				}
			} break;
		case 25:
			{
				// Gamepad B Released
				CameraControllerBase->BIsPressed = false;
				
			} break;
		case 26:
			{
				CameraControllerBase->XIsPressed = true;

				if(CameraControllerBase->RShoulderPressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Activating OffensiveAbilityID for unit: %s"), *SelectedUnit->GetName());
							OnAbilityInputDetected(SelectedUnit->OffensiveAbilityID, SelectedUnit, SelectedUnit->OffensiveAbilities);
						}
					}
				}else if(CameraControllerBase->LShoulderPressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							OnAbilityInputDetected(EGASAbilityInputID::AbilityFive, SelectedUnit, SelectedUnit->DefaultAbilities);
						}
					}
				}else if(CameraControllerBase->LShoulder2Pressed)
				{
					OnSimulatedMouseClick();
				}else
				{
					// ScatterMine
					CameraControllerBase->CPressed();
				}
				// Gamepad X
			} break;
		case 27:
			{
				CameraControllerBase->XIsPressed = false;
			} break;
		case 28:
			{
				// Gamepad Y

				//UE_LOG(LogTemp, Warning, TEXT("Gamepad Y Pressed!"));
				CameraControllerBase->YIsPressed = true;
				if(CameraControllerBase->RShoulderPressed)
				{

					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Activating DefensiveAbilityID for unit: %s"), *SelectedUnit->GetName());
							OnAbilityInputDetected(SelectedUnit->DefensiveAbilityID, SelectedUnit, SelectedUnit->DefensiveAbilities);
						}
					}
				}else if(CameraControllerBase->LShoulderPressed)
				{
					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							OnAbilityInputDetected(EGASAbilityInputID::AbilityThree, SelectedUnit, SelectedUnit->DefaultAbilities);
						}
					}
				}else if(!CameraControllerBase->LShoulder2Pressed && !CameraControllerBase->RShoulder2Pressed)
				{

					for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
					{
						if (SelectedUnit)
						{
							//UE_LOG(LogTemp, Warning, TEXT("Activating AbilityOne for unit: %s"), *SelectedUnit->GetName());
							OnAbilityInputDetected(EGASAbilityInputID::AbilityOne, SelectedUnit, SelectedUnit->DefaultAbilities);
						}
					}
				}
				
				//CameraControllerBase->QPressed();
			} break;
		case 29:
			{
				//UE_LOG(LogTemp, Warning, TEXT("Gamepad Y Released"));
				CameraControllerBase->YIsPressed = false;
				//CameraControllerBase->QReleased();
				
			} break;
		case 30:
			{
				// Joystick 2 X
				//UE_LOG(LogTemp, Warning, TEXT("Joystick 2 X"));
				float FValue = InputActionValue.Get<float>();
				JoyStick2X = FValue;
				if(CameraControllerBase->RShoulder2Pressed)
				{
					if(FValue >= 1)
					{
						SwitchAxis = true;
					}
					else if(FValue <= -1)
					{
						SwitchAxis = false;
					}
				}else
				{
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
				}
			} break;
		case 31:
			{
				//  Joystick 2 Y
				float FValue = InputActionValue.Get<float>();
				JoyStick2Y = FValue;

		
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
				CameraControllerBase->LShoulderPressed = true;
				//UE_LOG(LogTemp, Warning, TEXT("LShoulderPressed"));
			} break;
		case 33:
			{
				//  R Shoulder
				CameraControllerBase->RShoulderPressed = true;
				//UE_LOG(LogTemp, Warning, TEXT("RShoulderPressed"));
			} break;
		case 34:
			{
				//  L Shoulder	
				CameraControllerBase->LShoulderPressed = false;
				//UE_LOG(LogTemp, Warning, TEXT("LShoulderReleased"));
			} break;
		case 35:
			{
				//  R Shoulder
				CameraControllerBase->RShoulderPressed = false;
				//UE_LOG(LogTemp, Warning, TEXT("RShoulderReleased"));
			} break;
		case 36:
			{
				//  L Shoulder 2
				//UE_LOG(LogTemp, Warning, TEXT("L Shoulder 2"));
				CameraControllerBase->LShoulder2Pressed = true;

				if(!CameraControllerBase->RShoulder2Pressed)
				for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						if(!LevelWidgetToggled)
						{
							SetExtendedUserWidget(SelectedUnit);
							LevelWidgetToggled = true;
						}
					}
				}
				if(CameraControllerBase->RShoulder2Pressed && CameraControllerBase->LShoulder2Pressed )
					CameraControllerBase->RPressed();
			
			} break;
		case 37:
			{
				//  R Shoulder 2
				//UE_LOG(LogTemp, Warning, TEXT("R Shoulder 2"));
				CameraControllerBase->RShoulder2Pressed = true;
				if(CameraControllerBase->RShoulder2Pressed && CameraControllerBase->LShoulder2Pressed )
					CameraControllerBase->RPressed();
			} break;
		case 38:
			{
				//  L Shoulder 2
				//UE_LOG(LogTemp, Warning, TEXT("L Shoulder 2 Released"));
				CameraControllerBase->LShoulder2Pressed = false;

				
				for (AUnitBase* SelectedUnit : CameraControllerBase->SelectedUnits)
				{
					if (SelectedUnit)
					{
						if(LevelWidgetToggled)
						{
							SetExtendedUserWidget(nullptr);
							LevelWidgetToggled = false;
						}
					}
				}
				
			} break;
		case 39:
			{
				//  R Shoulder 2
				//UE_LOG(LogTemp, Warning, TEXT("R Shoulder 2 Released"));
				CameraControllerBase->RShoulder2Pressed = false;
			} break;
		case 40:
			{
				//UE_LOG(LogTemp, Warning, TEXT("L Trigger Pressed"));
				CameraControllerBase->SetDropJumpMine(true);
			} break;
		case 41:
			{
				//UE_LOG(LogTemp, Warning, TEXT("L Trigger Released"));
				CameraControllerBase->SetDropJumpMine(false);
			} break;
		case 42:
			{
				ShowControlWidget();
			} break;
		case 43:
			{
				HideControlWidget();
			} break;
		case 44:
			{
				float IValue = -1.0f;
				if(!SwitchAxis)
				{
					int Angles[4] = {0, 2, 1, 3};
					JoyStickMoveCamDependent(CameraControllerBase, IValue, Angles);
				}
			} break;
		case 45:
			{
				float IValue = 1.0f;
				if(!SwitchAxis)
				{
					int Angles[4] = {0, 2, 1, 3};
					JoyStickMoveCamDependent(CameraControllerBase, IValue, Angles);
				}
				
			} break;
		case 46:
			{
				float IValue = 1.0f;

				if(SwitchAxis)
				{
					int Angles[4] = {1, 3, 2, 0};
					JoyStickMoveCamDependent(CameraControllerBase, IValue, Angles);
				}
			} break;
		case 47:
			{
				float IValue = -1.0f;
				if(SwitchAxis)
				{
					int Angles[4] = {1, 3, 2, 0};
					JoyStickMoveCamDependent(CameraControllerBase, IValue, Angles);
				}
			} break;
		case 48:
			{
				if(CameraControllerBase->RShoulder2Pressed)
				{
						SwitchAxis = true;
				}else
				{
						CameraControllerBase->CamIsRotatingLeft = false;
						CameraControllerBase->CamIsRotatingRight = true;
				}
			} break;
		case 49:
			{
				if(CameraControllerBase->RShoulder2Pressed)
				{
						SwitchAxis = false;
				}else
				{
						CameraControllerBase->CamIsRotatingRight = false;
						CameraControllerBase->CamIsRotatingLeft = true;
				}
			} break;
		}
		
	}

}
