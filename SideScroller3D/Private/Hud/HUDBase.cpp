// Copyright 2023 Silvan Teufel / Teufel-Engineering.com All Rights Reserved.

#include "Hud/HUDBase.h"
#include "Blueprint/UserWidget.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Controller/CameraControllerBase.h"
#include "Elements/Actor/ActorElementData.h"
#include "GeometryCollection/GeometryCollectionSimulationTypes.h"
#include "Kismet/GameplayStatics.h"


void AHUDBase::DrawHUD()
{

	if (bSelectFriendly) {

		if(CharacterIsUnSelectable)
		DeselectAllUnits();

		if(CharacterIsUnSelectable)
		SelectedUnits.Empty();
		
		CurrentPoint = GetMousePos2D();

		if (abs(InitialPoint.X - CurrentPoint.X) >= 2) {

			DrawRect(FLinearColor(0, 0, 1, .15f),
				InitialPoint.X,
				InitialPoint.Y,
				CurrentPoint.X - InitialPoint.X,
				CurrentPoint.Y - InitialPoint.Y);

			const float LengthLineA = abs(InitialPoint.Y - CurrentPoint.Y);
			const float LengthLineB = abs(InitialPoint.X - CurrentPoint.X);
			FVector2D LineCenterPointA;
			FVector2D LineCenterPointB;
			FVector2D InitialSelectionPoint;
			FVector2D CurrentSelectionPoint;

			if (InitialPoint.Y < CurrentPoint.Y && InitialPoint.X < CurrentPoint.X) {
				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y + (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X - (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);

			}
			else if (InitialPoint.Y < CurrentPoint.Y && InitialPoint.X > CurrentPoint.X) {

				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y + (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X + (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);

			}
			else if (InitialPoint.Y > CurrentPoint.Y && InitialPoint.X < CurrentPoint.X) {
				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y - (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X - (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);
			}
			else if (InitialPoint.Y > CurrentPoint.Y && InitialPoint.X > CurrentPoint.X) {
				LineCenterPointA.X = InitialPoint.X;
				LineCenterPointB.Y = CurrentPoint.Y;

				LineCenterPointA.Y = InitialPoint.Y - (LengthLineA / 2);
				LineCenterPointB.X = CurrentPoint.X + (LengthLineB / 2);

				InitialSelectionPoint.X = LineCenterPointB.X + ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				InitialSelectionPoint.Y = LineCenterPointA.Y - ((LengthLineA * RectangleScaleSelectionFactor) / 2);

				CurrentSelectionPoint.X = LineCenterPointB.X - ((LengthLineB * RectangleScaleSelectionFactor) / 2);
				CurrentSelectionPoint.Y = LineCenterPointA.Y + ((LengthLineA * RectangleScaleSelectionFactor) / 2);
			}


			DrawRect(FLinearColor(0, 1, 0, .15f),
				InitialSelectionPoint.X,
				InitialSelectionPoint.Y,
				CurrentSelectionPoint.X - InitialSelectionPoint.X,
				CurrentSelectionPoint.Y - InitialSelectionPoint.Y);



			TArray <AUnitBase*> NewUnitBases;

			GetActorsInSelectionRectangle<AUnitBase>(InitialSelectionPoint, CurrentSelectionPoint, NewUnitBases, false, false);
			
			for (int32 i = 0; i < NewUnitBases.Num(); i++) {


		

		
				ACameraControllerBase* Controller = Cast<ACameraControllerBase>(GetOwningPlayerController());
			
				if(Controller && (NewUnitBases[i]->TeamId == Controller->SelectableTeamId || Controller->SelectableTeamId == 0)) // && IsActorInsideRec(IPoint, CPoint, ALocation) // && IsActorInsideRec(IPoint, CPoint, ALocation)
					{
					NewUnitBases[i]->SetSelected();
					SelectedUnits.Emplace(NewUnitBases[i]);
					}
			}

			NewUnitBases.Empty();









			/*
			
			TArray <AUnitBase*> NewUnitBases;

			GetActorsInSelectionRectangle<AUnitBase>(InitialSelectionPoint, CurrentSelectionPoint, NewUnitBases, false, false);

			if(NewUnitBases.Num() || CharacterIsUnSelectable)
			{
				if(NewUnitBases.Num() && NewUnitBases[0]->IsFriendly)
				{
					for (int32 i = 0; i < SelectedUnits.Num(); i++)
					{
						SelectedUnits[i]->SetDeselected();
					}
					SelectedUnits.Empty();
				}
				
				for (int32 i = 0; i < NewUnitBases.Num(); i++) {
					
					if(NewUnitBases[i]->IsFriendly ) // && IsActorInsideRec(IPoint, CPoint, ALocation) // && IsActorInsideRec(IPoint, CPoint, ALocation)
						{
						NewUnitBases[i]->SetSelected();
						SelectedUnits.Emplace(NewUnitBases[i]);
						}
				}
			}else if(CharacterIsUnSelectable)
			{
				if(SelectedUnits.Num())
				{
					for (int32 i = 0; i < SelectedUnits.Num(); i++)
					{
						SelectedUnits[i]->SetDeselected();
					}
					SelectedUnits.Empty();
				}
			}
			NewUnitBases.Empty();
			*/

		}

	}
}

void AHUDBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	MoveUnitsThroughWayPoints(SelectedUnits);

}

void AHUDBase::BeginPlay()
{
	Super::BeginPlay();
	GetAllMouseBots();
	GetAllUnits();
}

void AHUDBase::AssignNewHighestIndex(AUnitBase* Unit)
{
	HighestUnitIndex++;
	Unit->SetUnitIndex(HighestUnitIndex);
	//UE_LOG(LogTemp, Warning, TEXT("Assigned UnitINDEX! %d"), Unit->UnitIndex);
}


void AHUDBase::GetAllUnits()
{
	TArray <AActor*> AllActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AUnitBase::StaticClass(), AllActors);

	for(int i = 0; i < AllActors.Num(); i++)
	{
		// Filter Actors
		AUnitBase* NewUnitBase = Cast<AUnitBase>(AllActors[i]);
		if(NewUnitBase) // && IsActorInsideRec(IPoint, CPoint, ALocation) // && IsActorInsideRec(IPoint, CPoint, ALocation)
		{
			if(NewUnitBase->TeamId == 1)
			{
				AssignNewHighestIndex(NewUnitBase);
				NewUnitBase->SetSelected();
				SelectedUnits.Add(NewUnitBase);
			}
			AllUnits.Emplace(NewUnitBase);
		}
	}
}

void AHUDBase::GetAllMouseBots()
{
	TArray <AActor*> AllActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMouseBotBase::StaticClass(), AllActors);

	for(int i = 0; i < AllActors.Num(); i++)
	{
		// Filter Actors
		MouseBots.Emplace(Cast<AMouseBotBase>(AllActors[i]));
	}
}

FVector2D AHUDBase::GetMousePos2D()
{
	float PosX;
	float PosY;
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	return FVector2D(PosX, PosY);
}

void AHUDBase::RemoveMouseBot(AMouseBotBase* MouseBotBase)
{
	MouseBots.Remove(MouseBotBase);
}

void AHUDBase::MoveUnitsThroughWayPoints(TArray <AUnitBase*> Units)
{
	for (int32 i = 0; i < Units.Num(); i++) {

		if(Units[i])
		if (Units[i]->RunLocationArray.Num()) {

			FVector ActorLocation = Units[i]->GetActorLocation();

			if (ActorLocation.Equals(Units[i]->RunLocationArray[Units[i]->RunLocationArrayIterator], 200)) { 
				Units[i]->RunLocationArrayIterator++;
				if (Units[i]->RunLocationArrayIterator < Units[i]->RunLocationArray.Num()) {
					
					Units[i]->RunLocation = Units[i]->RunLocationArray[Units[i]->RunLocationArrayIterator];
					Units[i]->SetUnitState(UnitData::Run);
				}
				else {
					Units[i]->RunLocationArray.Empty();
					Units[i]->RunLocationArrayIterator = 0;
				}
			}
		}


	}
}


void AHUDBase::SetUnitSelected(AUnitBase* Unit)
{
	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		SelectedUnits[i]->SetDeselected();
	}

	SelectedUnits.Empty();
	SelectedUnits.Add(Unit);

	for (int32 i = 0; i < SelectedUnits.Num(); i++) {
		SelectedUnits[i]->SetSelected();
	}
}

void AHUDBase::DeselectAllUnits()
{
	if(CharacterIsUnSelectable)
		for (int32 i = 0; i < SelectedUnits.Num(); i++) {
			if(SelectedUnits[i])
				SelectedUnits[i]->SetDeselected();
		}

	if(CharacterIsUnSelectable)
		SelectedUnits.Empty();
}

void AHUDBase::ControllDirectionToMouse(AActor* Units, FHitResult Hit)
{
	FVector CharacterDirectionVector = Units->GetActorForwardVector();
	FVector ActorLocation = Units->GetActorLocation();

	float AngleDiff = CalcAngle(CharacterDirectionVector, Hit.Location - ActorLocation);

	FQuat QuadRotation;

	if ((AngleDiff > 2.f && AngleDiff < 179) || (AngleDiff <= -179.f && AngleDiff > -359.f)) {
		AngleDiff > 50.f || AngleDiff < -50.f ? QuadRotation = FQuat(FRotator(0.f, -20.0f, 0.f)) : QuadRotation = FQuat(FRotator(0.f, -4.0f, 0.f));
		
		Units->AddActorLocalRotation(QuadRotation, false, 0, ETeleportType::None);
	}
	else if ((AngleDiff < -2.f && AngleDiff > -179.f) || (AngleDiff >= 180.f && AngleDiff < 359.f)) {
		AngleDiff > 50.f || AngleDiff < -50.f ? QuadRotation = FQuat(FRotator(0.f, 20.0f, 0.f)) : QuadRotation = FQuat(FRotator(0.f, 4.0f, 0.f));
		Units->AddActorLocalRotation(QuadRotation, false, 0, ETeleportType::None);
	}
}

bool AHUDBase::IsActorInsideRec(FVector InPoint, FVector CuPoint, FVector ALocation)
{	
	if(InPoint.X < ALocation.X && ALocation.X < CuPoint.X && InPoint.Y < ALocation.Y && ALocation.Y < CuPoint.Y) return true;
	if(InPoint.X > ALocation.X && ALocation.X > CuPoint.X && InPoint.Y > ALocation.Y && ALocation.Y > CuPoint.Y) return true;
	if(InPoint.X < ALocation.X && ALocation.X < CuPoint.X && InPoint.Y > ALocation.Y && ALocation.Y > CuPoint.Y) return true;
	if(InPoint.X > ALocation.X && ALocation.X > CuPoint.X && InPoint.Y < ALocation.Y && ALocation.Y < CuPoint.Y) return true;

	return false;
}
