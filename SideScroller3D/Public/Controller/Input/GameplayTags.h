// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

class UGameplayTagsManager;

struct FGameplayTags
{
public:

	static const FGameplayTags& Get() { return GameplayTags; }

	static void InitializeNativeTags();

	//Input Tags
	FGameplayTag InputTag_LeftClick_Pressed;
	FGameplayTag InputTag_LeftClick_Released;
	FGameplayTag InputTag_RightClick_Pressed;
	FGameplayTag InputTag_Ctrl_Pressed;
	FGameplayTag InputTag_Ctrl_Released;
	FGameplayTag InputTag_Alt_Pressed;
	FGameplayTag InputTag_Alt_Released;
	FGameplayTag InputTag_Tab_Pressed;
	FGameplayTag InputTag_Tab_Released;
	FGameplayTag InputTag_Shift_Pressed;
	FGameplayTag InputTag_Shift_Released;
	FGameplayTag InputTag_Space_Pressed;
	FGameplayTag InputTag_Space_Released;

	FGameplayTag InputTag_Q_Pressed;
	FGameplayTag InputTag_E_Pressed;
	FGameplayTag InputTag_W_Pressed;
	FGameplayTag InputTag_A_Pressed;
	FGameplayTag InputTag_S_Pressed;
	FGameplayTag InputTag_D_Pressed;

	FGameplayTag InputTag_Q_Released;
	FGameplayTag InputTag_E_Released;
	FGameplayTag InputTag_W_Released;
	FGameplayTag InputTag_A_Released;
	FGameplayTag InputTag_S_Released;
	FGameplayTag InputTag_D_Released;

	FGameplayTag InputTag_Y_Pressed;
	FGameplayTag InputTag_X_Pressed;

	FGameplayTag InputTag_Y_Released;
	FGameplayTag InputTag_X_Released;
	
	FGameplayTag InputTag_R_Pressed;
	FGameplayTag InputTag_L_Pressed;
	FGameplayTag InputTag_F_Pressed;

	FGameplayTag InputTag_C_Pressed;
	FGameplayTag InputTag_G_Pressed;
	FGameplayTag InputTag_T_Pressed;
	FGameplayTag InputTag_P_Pressed;
	FGameplayTag InputTag_O_Pressed;
	
	FGameplayTag InputTag_Joystick_1_X;
	FGameplayTag InputTag_Joystick_1_Y;
	FGameplayTag InputTag_Joystick_2_X;
	FGameplayTag InputTag_Joystick_2_Y;
	
	FGameplayTag InputTag_Gamepad_Up;
	FGameplayTag InputTag_Gamepad_Down;
	FGameplayTag InputTag_Gamepad_Left;
	FGameplayTag InputTag_Gamepad_Right;
	
	FGameplayTag InputTag_Gamepad_A_Pressed;
	FGameplayTag InputTag_Gamepad_A_Released;

	FGameplayTag InputTag_Gamepad_B_Pressed;
	FGameplayTag InputTag_Gamepad_B_Released;

	FGameplayTag InputTag_Gamepad_X_Pressed;
	FGameplayTag InputTag_Gamepad_X_Released;

	FGameplayTag InputTag_Gamepad_Y_Pressed;
	FGameplayTag InputTag_Gamepad_Y_Released;

	FGameplayTag InputTag_Left_Shoulder;
	FGameplayTag InputTag_Right_Shoulder;

	FGameplayTag InputTag_Left_Shoulder_Released;
	FGameplayTag InputTag_Right_Shoulder_Released;
	
	FGameplayTag InputTag_Left_Shoulder_2;
	FGameplayTag InputTag_Right_Shoulder_2;

	FGameplayTag InputTag_Left_Shoulder_2_Released;
	FGameplayTag InputTag_Right_Shoulder_2_Released;
	
	FGameplayTag InputTag_Left_Trigger_Pressed;
	FGameplayTag InputTag_Left_Trigger_Released;

	FGameplayTag InputTag_Arrow_Left;
	FGameplayTag InputTag_Arrow_Right;
	FGameplayTag InputTag_Arrow_Up;
	FGameplayTag InputTag_Arrow_Down;


	FGameplayTag InputTag_Scroll_D1;
	FGameplayTag InputTag_Scroll_D2;
	
	FGameplayTag InputTag_Middle_Mouse_Pressed;
	FGameplayTag InputTag_Middle_Mouse_Released;
	
	FGameplayTag InputTag_1_Pressed;
	FGameplayTag InputTag_2_Pressed;
	FGameplayTag InputTag_3_Pressed;
	FGameplayTag InputTag_4_Pressed;
	FGameplayTag InputTag_5_Pressed;
	FGameplayTag InputTag_6_Pressed;
	
protected:
	//Registers all of the tags with the GameplayTags Manager
	void AddAllTags(UGameplayTagsManager& Manager);

	//Helper function used by AddAllTags to register a single tag with the GameplayTags Manager
	void AddTag(FGameplayTag& OutTag, const ANSICHAR* TagName, const ANSICHAR* TagComment);

private:

	static FGameplayTags GameplayTags;
};