
#include "Widgets/AbilityChooser.h"

#include "Controller/ControllerBase.h"

void UAbilityChooser::NativeConstruct()
{
    Super::NativeConstruct();
     UsedAbilityPointsTextArray.Empty();
    InitializeButtonArray(ButtonPreFixes[0], OffensiveAbilityButtons);
    InitializeButtonArray(ButtonPreFixes[1], DefensiveAbilityButtons);
    InitializeButtonArray(ButtonPreFixes[2], AttackAbilityButtons);
    InitializeButtonArray(ButtonPreFixes[3], ThrowAbilityButtons);
    InitializeSingleButton("Save", Save);
    InitializeSingleButton("Load", Load);
    InitializeSingleButton("ResetAbilityButton", ResetAbilityButton);
    SetVisibility(ESlateVisibility::Hidden);
}

void UAbilityChooser::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    UpdateAbilityDisplay();
}

void UAbilityChooser::SetOwnerActor(AAbilityUnit* NewOwner)
{
    OwnerAbilityUnit = NewOwner;
}

void UAbilityChooser::UpdateAbilityDisplay()
{
    if (OwnerAbilityUnit)
    {
        if (OffensiveAbilityText)
        {
            OffensiveAbilityText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OwnerAbilityUnit->OffensiveAbilityID))));
        }
        if (DefensiveAbilityText)
        {
            DefensiveAbilityText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OwnerAbilityUnit->DefensiveAbilityID))));
        }
        if (AttackAbilityText)
        {
            AttackAbilityText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OwnerAbilityUnit->AttackAbilityID))));
        }
        if (ThrowAbilityText)
        {
            ThrowAbilityText->SetText(FText::FromString(FString::FromInt(static_cast<int32>(OwnerAbilityUnit->ThrowAbilityID))));
        }
        
        if (AvailableAbilityPointsText)
        {
            AvailableAbilityPointsText->SetText(FText::AsNumber(OwnerAbilityUnit->LevelData.AbilityPoints));
        }

        if (UsedAbilityPointsText)
        {
            UsedAbilityPointsText->SetText(FText::AsNumber(OwnerAbilityUnit->LevelData.UsedAbilityPoints));
        }

        // Update UsedAbilityPointsTextArray
        for (int32 Index = 0; Index < UsedAbilityPointsTextArray.Num(); Index++)
        {
            if (UsedAbilityPointsTextArray[Index])
            {
                int32 UsedPoints = OwnerAbilityUnit->LevelData.UsedAbilityPointsArray.IsValidIndex(Index) ? OwnerAbilityUnit->LevelData.UsedAbilityPointsArray[Index] : 0;
                UsedAbilityPointsTextArray[Index]->SetText(FText::AsNumber(UsedPoints));
            }
        }
    }
}

FString UAbilityChooser::GetEnumValueAsString(const FString& EnumName, int32 EnumValue)
{
    UEnum* Enum = FindObject<UEnum>(nullptr, *EnumName, true);
    if (!Enum)
    {
        return FString("Invalid");
    }
    return Enum->GetNameByValue(EnumValue).ToString();
}

UButton* UAbilityChooser::GetButtonUnderCursor() const
{
    // Check OffensiveAbilityButtons
    for (UButton* Button : OffensiveAbilityButtons)
    {
        if (Button && Button->IsHovered())
        {
            return Button;
        }
    }

    // Check DefensiveAbilityButtons
    for (UButton* Button : DefensiveAbilityButtons)
    {
        if (Button && Button->IsHovered())
        {
            return Button;
        }
    }

    // Check AttackAbilityButtons
    for (UButton* Button : AttackAbilityButtons)
    {
        if (Button && Button->IsHovered())
        {
            return Button;
        }
    }

    // Check ThrowAbilityButtons
    for (UButton* Button : ThrowAbilityButtons)
    {
        if (Button && Button->IsHovered())
        {
            return Button;
        }
    }

    if (Save && Save->IsHovered())
    {
        return Save;
    }

    if (Load && Load->IsHovered())
    {
        return Load;
    }

    if (ResetAbilityButton && ResetAbilityButton->IsHovered())
    {
        return ResetAbilityButton;
    }

    // If no buttons are hovered, return nullptr
    return nullptr;
}

/*
FString UAbilityChooser::GetEnumValueAsString(const FString& EnumName, int32 EnumValue)
{
    UEnum* Enum = FindObject<UEnum>(ANY_PACKAGE, *EnumName, true);
    if (!Enum)
    {
        return FString("Invalid");
    }
    return Enum->GetNameByValue(EnumValue).ToString();
}*/

void UAbilityChooser::InitializeButtonArray(const FString& ButtonPrefix, TArray<UButton*>& ButtonArray)
{
    ButtonArray.Empty();
    for (int32 Index = 0; Index < ButtonInitCount; ++Index) // Assuming you have 4 buttons per category
    {
        FString ButtonName = FString::Printf(TEXT("%s_%d"), *ButtonPrefix, Index);
        UButton* Button = Cast<UButton>(GetWidgetFromName(FName(*ButtonName)));

            if (Button)
            {
                ButtonArray.Add(Button);
            }
     }
    
    FString TextBlockName = FString::Printf(TEXT("UsedAbilityPointArray_%s"), *ButtonPrefix); // Modify this based on your actual naming convention
    UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(FName(*TextBlockName)));
    if (TextBlock)
    {
        UsedAbilityPointsTextArray.Add(TextBlock);
    }
}


void UAbilityChooser::InitializeSingleButton(const FString& ButtonName, UButton* Button)
{
        UButton* NewButton = Cast<UButton>(GetWidgetFromName(FName(*ButtonName)));

        if (NewButton)
        {
            Button = NewButton;
        }
}