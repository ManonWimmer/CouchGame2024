// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateRail.h"

#include "MaterialHLSLTree.h"
#include "Components/SphereComponent.h"
#include "PinballElements/Elements/RailElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"

EPlayerBallStateID UPlayerBallStateRail::GetStateID() const
{
	return EPlayerBallStateID::Rail;
}

void UPlayerBallStateRail::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateRail::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState : Rail");

	CurrentTimeInRail = 0.f;
	
	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			CurrentRailElement = Pawn->BehaviorElementReactions->CurrentRailElement;
		}
	}
}

void UPlayerBallStateRail::StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter)
{
	Super::StateEnter(PreviousState, InFloatParameter);


	DirectionRail = InFloatParameter;
	
	EnterRail();

	if (CurrentRailElement == nullptr)
	{
		ExitRail();
	}
}

void UPlayerBallStateRail::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->CurrentRailElement = nullptr;
		}
	}
	
	
	CurrentRailElement = nullptr;
}

void UPlayerBallStateRail::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	HandleRailProgressLocation(DeltaTime);
}

void UPlayerBallStateRail::EnterRail()
{
	if (Pawn == nullptr)	return;

	Pawn->ResetMovement();
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(false);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void UPlayerBallStateRail::HandleRailProgressLocation(float DeltaTime)
{
	if (Pawn == nullptr)	return;
	if (CurrentRailElement == nullptr)	return;

	if (CurrentTimeInRail - EndProgressOffset >= ProgressRailDuration)
	{
		ExitRail();
	}
	else
	{
		CurrentTimeInRail += DeltaTime;

		float Percent = CurrentTimeInRail / ProgressRailDuration;

		if (DirectionRail >= 0.f)
		{
			float InversePercent = 1.f - Percent;
		
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(InversePercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 10.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
		else
		{
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(Percent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 10.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
	}
}

void UPlayerBallStateRail::ExitRail()
{
	if (Pawn == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;
	if (StateMachine == nullptr)	return;
	
	Pawn->SphereCollision->SetSimulatePhysics(true);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}
