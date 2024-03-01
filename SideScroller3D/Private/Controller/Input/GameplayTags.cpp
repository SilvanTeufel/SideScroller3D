// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Controller/Input/GameplayTags.h"
#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

FGameplayTags FGameplayTags::GameplayTags;

void FGameplayTags::InitializeNativeTags()
{
	UGameplayTagsManager& GameplayTagsManager = UGameplayTagsManager::Get();

	GameplayTags.AddAllTags(GameplayTagsManager);

	GameplayTagsManager.DoneAddingNativeTags();
}

void FGameplayTags::AddAllTags(UGameplayTagsManager& Manager)
{
	AddTag(InputTag_LeftClick_Pressed, "InputTag.LeftClick_Pressed", "LeftClick_Pressed");
	AddTag(InputTag_LeftClick_Released, "InputTag.LeftClick_Released", "LeftClick_Released");
	
	AddTag(InputTag_RightClick_Pressed, "InputTag.RightClick_Pressed", "RightClick_Pressed");
	
	AddTag(InputTag_Ctrl_Pressed, "InputTag.Ctrl_Pressed", "Ctrl_Pressed");
	AddTag(InputTag_Ctrl_Released, "InputTag.Ctrl_Released", "Ctrl_Released");

	AddTag(InputTag_Tab_Pressed, "InputTag.Tab_Pressed", "Tab_Pressed");
	AddTag(InputTag_Tab_Released, "InputTag.Tab_Released", "Tab_Released");
	
	AddTag(InputTag_Alt_Pressed, "InputTag.Alt_Pressed", "Alt_Pressed");
	AddTag(InputTag_Alt_Released, "InputTag.Alt_Released", "Alt_Released");
	
	AddTag(InputTag_Shift_Pressed, "InputTag.Shift_Pressed", "Shift_Pressed");
	AddTag(InputTag_Shift_Released, "InputTag.Shift_Released", "Shift_Released");

	AddTag(InputTag_Space_Pressed, "InputTag.Space_Pressed", "Space_Pressed");
	AddTag(InputTag_Space_Released, "InputTag.Space_Released", "Space_Released");
	
	AddTag(InputTag_Q_Pressed, "InputTag.Q_Pressed", "Q_Pressed");
	AddTag(InputTag_E_Pressed, "InputTag.E_Pressed", "E_Pressed");
	AddTag(InputTag_W_Pressed, "InputTag.W_Pressed", "W_Pressed");
	AddTag(InputTag_A_Pressed, "InputTag.A_Pressed", "A_Pressed");
	AddTag(InputTag_S_Pressed, "InputTag.S_Pressed", "S_Pressed");
	AddTag(InputTag_D_Pressed, "InputTag.D_Pressed", "D_Pressed");

	AddTag(InputTag_Q_Released, "InputTag.Q_Released", "Q_Released");
	AddTag(InputTag_E_Released, "InputTag.E_Released", "E_Released");
	AddTag(InputTag_W_Released, "InputTag.W_Released", "W_Released");
	AddTag(InputTag_A_Released, "InputTag.A_Released", "A_Released");
	AddTag(InputTag_S_Released, "InputTag.S_Released", "S_Released");
	AddTag(InputTag_D_Released, "InputTag.D_Released", "D_Released");

	AddTag(InputTag_Y_Pressed, "InputTag.Y_Pressed", "Y_Pressed");
	AddTag(InputTag_X_Pressed, "InputTag.X_Pressed", "X_Pressed");

	AddTag(InputTag_Y_Released, "InputTag.Y_Released", "Y_Released");
	AddTag(InputTag_X_Released, "InputTag.X_Released", "X_Released");
	
	AddTag(InputTag_E_Pressed, "InputTag.E_Pressed", "E_Pressed");
	AddTag(InputTag_R_Pressed, "InputTag.R_Pressed", "R_Pressed");
	AddTag(InputTag_L_Pressed, "InputTag.L_Pressed", "L_Pressed");
	AddTag(InputTag_F_Pressed, "InputTag.F_Pressed", "F_Pressed");


	AddTag(InputTag_R_Pressed, "InputTag.R_Pressed", "R_Pressed");

	AddTag(InputTag_F_Pressed, "InputTag.F_Pressed", "F_Pressed");

	AddTag(InputTag_C_Pressed, "InputTag.C_Pressed", "C_Pressed");
	
	AddTag(InputTag_G_Pressed, "InputTag.G_Pressed", "G_Pressed");
	
	AddTag(InputTag_T_Pressed, "InputTag.T_Pressed", "T_Pressed");
	
	AddTag(InputTag_P_Pressed, "InputTag.P_Pressed", "P_Pressed");
	AddTag(InputTag_O_Pressed, "InputTag.O_Pressed", "O_Pressed");

	
	AddTag(InputTag_Joystick_1_X, "InputTag.Joystick_1_X", "Joystick_1_X");
	AddTag(InputTag_Joystick_1_Y, "InputTag.Joystick_1_Y", "Joystick_1_Y");

	AddTag(InputTag_Joystick_2_X, "InputTag.Joystick_2_X", "Joystick_2_X");
	AddTag(InputTag_Joystick_2_Y, "InputTag.Joystick_2_Y", "Joystick_2_Y");

	AddTag(InputTag_Gamepad_Up, "InputTag.Gamepad_Up", "Gamepad_Up");
	AddTag(InputTag_Gamepad_Down, "InputTag.Gamepad_Down", "Gamepad_Down");
	AddTag(InputTag_Gamepad_Left, "InputTag.Gamepad_Left", "Gamepad_Left");
	AddTag(InputTag_Gamepad_Right, "InputTag.Gamepad_Right", "Gamepad_Right");
	
	AddTag(InputTag_Gamepad_A_Pressed, "InputTag.Gamepad_A_Pressed", "Gamepad_A_Pressed");
	AddTag(InputTag_Gamepad_A_Released, "InputTag.Gamepad_A_Released", "Gamepad_A_Released");
	
	AddTag(InputTag_Gamepad_B_Pressed, "InputTag.Gamepad_B_Pressed", "Gamepad_B_Pressed");
	AddTag(InputTag_Gamepad_B_Released, "InputTag.Gamepad_B_Released", "Gamepad_B_Released");
	
	AddTag(InputTag_Gamepad_X_Pressed, "InputTag.Gamepad_X_Pressed", "Gamepad_X_Pressed");
	AddTag(InputTag_Gamepad_X_Released, "InputTag.Gamepad_X_Released", "Gamepad_X_Released");

	AddTag(InputTag_Gamepad_Y_Pressed, "InputTag.Gamepad_Y_Pressed", "Gamepad_Y_Pressed");
	AddTag(InputTag_Gamepad_Y_Released, "InputTag.Gamepad_Y_Released", "Gamepad_Y_Released");

	AddTag(InputTag_Left_Shoulder, "InputTag.Left_Shoulder", "Left_Shoulder");
	AddTag(InputTag_Right_Shoulder, "InputTag.Right_Shoulder", "Right_Shoulder");

	AddTag(InputTag_Left_Shoulder_Released, "InputTag.Left_Shoulder_Released", "Left_Shoulder_Released");
	AddTag(InputTag_Right_Shoulder_Released, "InputTag.Right_Shoulder_Released", "Right_Shoulder_Released");
	
	AddTag(InputTag_Left_Shoulder_2, "InputTag.Left_Shoulder_2", "Left_Shoulder_2");
	AddTag(InputTag_Right_Shoulder_2, "InputTag.Right_Shoulder_2", "Right_Shoulder_2");

	AddTag(InputTag_Left_Shoulder_2_Released, "InputTag.Left_Shoulder_2_Released", "Left_Shoulder_2_Released");
	AddTag(InputTag_Right_Shoulder_2_Released, "InputTag.Right_Shoulder_2_Released", "Right_Shoulder_2_Released");
	
	AddTag(InputTag_Left_Trigger_Pressed, "InputTag.Left_Trigger_Pressed", "Left_Trigger_Pressed");
	AddTag(InputTag_Left_Trigger_Released, "InputTag.Left_Trigger_Released", "Left_Trigger_Released");
	
	AddTag(InputTag_Arrow_Left, "InputTag.Arrow_Left", "Arrow_Left");
	AddTag(InputTag_Arrow_Right, "InputTag.Arrow_Right", "Arrow_Right");
	AddTag(InputTag_Arrow_Up, "InputTag.Arrow_Up", "Arrow_Up");
	AddTag(InputTag_Arrow_Down, "InputTag.Arrow_Down", "Arrow_Down");

	AddTag(InputTag_Scroll_D1, "InputTag.Scroll_D1", "Scroll_D1");
	AddTag(InputTag_Scroll_D2, "InputTag.Scroll_D2", "Scroll_D2");
	
	AddTag(InputTag_Middle_Mouse_Pressed, "InputTag.Middle_Mouse_Pressed", "Middle_Mouse_Pressed");
	AddTag(InputTag_Middle_Mouse_Released, "InputTag.Middle_Mouse_Released", "Middle_Mouse_Released");
	
	AddTag(InputTag_1_Pressed, "InputTag.1_Pressed", "1_Pressed");
	AddTag(InputTag_2_Pressed, "InputTag.2_Pressed", "2_Pressed");
	AddTag(InputTag_3_Pressed, "InputTag.3_Pressed", "3_Pressed");
	AddTag(InputTag_4_Pressed, "InputTag.4_Pressed", "4_Pressed");
	AddTag(InputTag_5_Pressed, "InputTag.5_Pressed", "5_Pressed");
	AddTag(InputTag_6_Pressed, "InputTag.6_Pressed", "6_Pressed");
	
}

void FGameplayTags::AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment)
{
	OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), FString(TEXT("(Native) ")) + FString(TagComment));
}