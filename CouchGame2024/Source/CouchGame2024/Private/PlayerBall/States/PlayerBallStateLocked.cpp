// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateLocked.h"

#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "PlayerBall/Datas/PlayerBallData.h"

EPlayerBallStateID UPlayerBallStateLocked::GetStateID() const
{
	return EPlayerBallStateID::Locked;
}

void UPlayerBallStateLocked::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);

	if (Pawn->GetPlayerBallData() != nullptr)
	{
		StartingStateMinForce = Pawn->GetPlayerBallData()->StartingStateMinForce;

		StartingStateMaxForce = Pawn->GetPlayerBallData()->StartingStateMaxForce;
		
		PercentAddByStartingSpam = Pawn->GetPlayerBallData()->PercentAddByStartingSpam;
	}
}

void UPlayerBallStateLocked::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);
}

void UPlayerBallStateLocked::StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter)
{
	Super::StateEnter(PreviousState, InFloatParameter);

	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, TEXT("PlayerState : Locked"));

	
	CurrentProgressStartForce = 0.f;
	
	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;
		}
		

		if (InFloatParameter != 0.f)
		{
			Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateLocked::AddStartForceToProgressValue);
			bIsLockedSpecial = true;
			
			if (Pawn->StartForceEffectWidget != nullptr)
			{
				Pawn->StartForceEffectWidget->SetHiddenInGame(false);

				Pawn->PlaySetProgressStartForceEffect(0.f);
			}
		}

	}
}

void UPlayerBallStateLocked::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);
	
	if (Pawn != nullptr)
	{
		if (bIsLockedSpecial)
		{
			if (Pawn->StartForceEffectWidget != nullptr)
			{
				Pawn->StartForceEffectWidget->SetHiddenInGame(true);
			}
			
			ImpulseInDirection();
		}

		bIsLockedSpecial = false;

		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateLocked::AddStartForceToProgressValue);
	}
}

void UPlayerBallStateLocked::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateLocked::AddStartForceToProgressValue(float InValue)
{
	CurrentProgressStartForce += PercentAddByStartingSpam;

	if (Pawn == nullptr)	return;

	Pawn->PlaySetProgressStartForceEffect(CurrentProgressStartForce);
}

void UPlayerBallStateLocked::ImpulseInDirection()
{
	if (Pawn == nullptr)	return;

	if (Pawn->SphereCollision == nullptr)	return;
	
	if (Pawn->BehaviorMovements == nullptr)	return;

	// Inverse values X and Y to match world orientation
	//float DirX = Pawn->BehaviorMovements->MoveYValue;
	//float DirY = Pawn->BehaviorMovements->MoveXValue;
	//FVector Dir = FVector(DirX, DirY, 0.f);

	FVector Dir = Pawn->BehaviorMovements->SpecialSpawnForceDir;

	
	if (Dir.X == 0.f && Dir.Y == 0.f)
	{
		float RandomX = UKismetMathLibrary::RandomIntegerInRange(-1.f, 1.f);
		float RandomY = UKismetMathLibrary::RandomIntegerInRange(-1.f, 1.f);
		
		Dir = FVector(RandomX, RandomY, 0.f);
	}

	Dir.Normalize();

	CurrentProgressStartForce = FMath::Clamp(CurrentProgressStartForce, 0.f, 1.f);
	
	float ForceMultiplier = FMath::Lerp(StartingStateMinForce, StartingStateMaxForce, CurrentProgressStartForce);
	
	Pawn->SphereCollision->AddImpulse(Dir * ForceMultiplier);
}
