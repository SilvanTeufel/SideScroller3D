// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.


#include "Animations/UnitBaseAnimInstance.h"
#include "Characters/Unit/ExtendedUnitBase.h"
#include "Characters/Unit/UnitBase.h"
#include "Core/UnitData.h"

UUnitBaseAnimInstance::UUnitBaseAnimInstance() {
	CharAnimState = UnitData::Idle;
}


void UUnitBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	AUnitBase* Actor = Cast<AUnitBase>(OwningActor);
}

void UUnitBaseAnimInstance::NativeUpdateAnimation(float Deltaseconds)
{
	Super::NativeUpdateAnimation(Deltaseconds);
	OwningActor = GetOwningActor();
	
	if (OwningActor != nullptr) {
		AUnitBase* UnitBase = Cast<AUnitBase>(OwningActor);

		if (UnitBase != nullptr) {
			
			CharAnimState = UnitBase->GetUnitState();
			SetAnimTableData(CharAnimState);

			if(LastAnimState != CharAnimState)
			{
				SoundTimer = 0.f;
				LastAnimState = CharAnimState;
			}

			if(Sound && UnitBase)
			{
				if(SoundTimer == 0.f)
					UGameplayStatics::PlaySoundAtLocation(UnitBase, Sound, UnitBase->GetActorLocation(), 1.f);

				SoundTimer += Deltaseconds;
			}
			
			switch (CharAnimState)
			{
			case UnitData::Attack:
				{
					AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(UnitBase);
					if(ExtendedUnitBase)
					{
						BlendPoint_1 -= (1/ExtendedUnitBase->AttackDuration)*AttackTempoScale;
						BlendPoint_2 -= (1/ExtendedUnitBase->AttackDuration)*AttackTempoScale;
					}
				}
				break;
			default:
				{

				}
				break;
			}
			
			AExtendedUnitBase* ExtendedUnitBase = Cast<AExtendedUnitBase>(UnitBase);
			if(ExtendedUnitBase)
			{
				if(ExtendedUnitBase->JumpLoad > 0.f)
				{
					switch (CharAnimState)
					{
						case UnitData::Run:
						{
								SetBlendPoints(UnitData::RunJumpLoad);
								USoundBase* JLSound = GetAnimSound(UnitData::RunJumpLoad);
								if(JLSound && ExtendedUnitBase && !JumpLoadSoundPlayed)
								{
									UGameplayStatics::PlaySoundAtLocation(ExtendedUnitBase, JLSound, ExtendedUnitBase->GetActorLocation(), 1.f);
									JumpLoadSoundPlayed = true;
								}
								
						}
						break;
						case UnitData::Idle:
						{
								SetBlendPoints(UnitData::IdleJumpLoad);
								USoundBase* JLSound = GetAnimSound(UnitData::RunJumpLoad);
								if(JLSound && ExtendedUnitBase && !JumpLoadSoundPlayed)
								{
									UGameplayStatics::PlaySoundAtLocation(ExtendedUnitBase, JLSound, ExtendedUnitBase->GetActorLocation(), 1.f);
									JumpLoadSoundPlayed = true;
								}
										
						}
						break;
					}
				}else
				{
					JumpLoadSoundPlayed = false;
				}
			}

			if(abs(CurrentBlendPoint_1-BlendPoint_1) <= Resolution_1) CurrentBlendPoint_1 = BlendPoint_1;
			else if(CurrentBlendPoint_1 < BlendPoint_1) CurrentBlendPoint_1 += TransitionRate_1;
			else if(CurrentBlendPoint_1 > BlendPoint_1) CurrentBlendPoint_1 += TransitionRate_1*(-1);

			if(abs(CurrentBlendPoint_2-BlendPoint_2) <= Resolution_2) CurrentBlendPoint_2 = BlendPoint_2;
			else if(CurrentBlendPoint_2 < BlendPoint_2) CurrentBlendPoint_2 += TransitionRate_2;
			else if(CurrentBlendPoint_2 > BlendPoint_2) CurrentBlendPoint_2 += TransitionRate_2*(-1);
			
		}
	}
}

void UUnitBaseAnimInstance::SetAnimTableData(TEnumAsByte<UnitData::EState> AnimState)
{
	if (AnimDataTable)
	{
		for(auto it : AnimDataTable->GetRowMap())
		{
			FString Key = it.Key.ToString();
			UnitAnimData = reinterpret_cast<FUnitAnimData*>(it.Value);
			if(UnitAnimData->AnimState == AnimState)
			{
				BlendPoint_1 = UnitAnimData->BlendPoint_1;
				BlendPoint_2 = UnitAnimData->BlendPoint_2;
				TransitionRate_1 = UnitAnimData->TransitionRate_1;
				TransitionRate_2 = UnitAnimData->TransitionRate_2;
				Resolution_1 = UnitAnimData->Resolution_1;
				Resolution_2 = UnitAnimData->Resolution_2;
				Sound = UnitAnimData->Sound;
			}
		}
		
	}
}


void UUnitBaseAnimInstance::SetBlendPoints(TEnumAsByte<UnitData::EState> AnimState)
{
	if (AnimDataTable)
	{
		for(auto it : AnimDataTable->GetRowMap())
		{
			FString Key = it.Key.ToString();
			UnitAnimData = reinterpret_cast<FUnitAnimData*>(it.Value);
			if(UnitAnimData->AnimState == AnimState)
			{
				BlendPoint_1 = UnitAnimData->BlendPoint_1;
				BlendPoint_2 = UnitAnimData->BlendPoint_2;
			}
		}
		
	}
}

USoundBase* UUnitBaseAnimInstance::GetAnimSound(TEnumAsByte<UnitData::EState> AnimState)
{
	if (AnimDataTable)
	{
		for(auto it : AnimDataTable->GetRowMap())
		{
			FString Key = it.Key.ToString();
			UnitAnimData = reinterpret_cast<FUnitAnimData*>(it.Value);
			if(UnitAnimData->AnimState == AnimState)
			{
				return UnitAnimData->Sound;
			}
		}
		
	}
	return nullptr;
}
