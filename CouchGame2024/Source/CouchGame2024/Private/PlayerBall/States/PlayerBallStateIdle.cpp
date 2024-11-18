// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateIdle.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"


// Sets default values for this component's properties
UPlayerBallStateIdle::UPlayerBallStateIdle()
{
	
}

EPlayerBallStateID UPlayerBallStateIdle::GetStateID() const
{
	return EPlayerBallStateID::Idle;
}

void UPlayerBallStateIdle::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateIdle::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("PlayerState : Idle"));
	
	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Idle") );

	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.AddDynamic(this, &UPlayerBallStateIdle::OnDeath);

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateIdle::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateIdle::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.AddDynamic(this, &UPlayerBallStateIdle::OnBumped);
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.AddDynamic(this, &UPlayerBallStateIdle::OnSnapped);

			Pawn->BehaviorElementReactions->OnRailReaction.AddDynamic(this, &UPlayerBallStateIdle::OnRail);
		}
		
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = true;
			Pawn->BehaviorGrapple->CanBeGrappled = true;
			
			Pawn->BehaviorGrapple->OnGrapplingActionStarted.AddDynamic(this, &UPlayerBallStateIdle::OnGrappling);
			Pawn->BehaviorGrapple->OnGrappledActionStarted.AddDynamic(this, &UPlayerBallStateIdle::OnGrappled);
		}

		if (Pawn->BehaviorPowerUp != nullptr)
		{
			Pawn->BehaviorPowerUp->OnUsePowerUpAction.AddDynamic(this, &UPlayerBallStateIdle::OnUsePowerUp);
		}
		
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateIdle::OnPunch);

	}
}

void UPlayerBallStateIdle::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.RemoveDynamic(this, &UPlayerBallStateIdle::OnDeath);

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateIdle::OnBumped);
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnSnapped);

			Pawn->BehaviorElementReactions->OnRailReaction.RemoveDynamic(this, &UPlayerBallStateIdle::OnRail);
		}

		if (Pawn->BehaviorPowerUp != nullptr)
		{
			Pawn->BehaviorPowerUp->OnUsePowerUpAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnUsePowerUp);
		}

		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->OnGrapplingActionStarted.RemoveDynamic(this, &UPlayerBallStateIdle::OnGrappling);
			Pawn->BehaviorGrapple->OnGrappledActionStarted.RemoveDynamic(this, &UPlayerBallStateIdle::OnGrappled);
		}
		
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnPunch);
	}
}

void UPlayerBallStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	OnMoveCheck();

	CheckFalling();
}

void UPlayerBallStateIdle::OnMoveCheck()	// Check if ball start moving
{
	if (Pawn == nullptr)	return;

	if (Pawn->BehaviorMovements == nullptr)	return;
	
	if (FMathf::Abs(Pawn->BehaviorMovements->MoveXValue) >= 0.1f || FMathf::Abs(Pawn->BehaviorMovements->MoveYValue) >= 0.1f)	// moving -> Move
	{
		if (StateMachine == nullptr)	return;

		StateMachine->ChangeState(EPlayerBallStateID::Move);
	}
}

void UPlayerBallStateIdle::CheckFalling()	// Check if ball falling
{
	if (Pawn == nullptr)	return;

	if (Pawn->BehaviorMovements == nullptr)	return;
	
	if (!Pawn->BehaviorMovements->IsGrounded())	// not on ground -> falling
	{
		StateMachine->ChangeState(EPlayerBallStateID::Fall);
	}
}

void UPlayerBallStateIdle::OnStunned(float StunnedValue)	// -> stunned
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateIdle::OnPunch(float PunchValue)	// -> punch
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Punch);
}

void UPlayerBallStateIdle::OnImpacted(float ImpactedValue)	// -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateIdle::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

void UPlayerBallStateIdle::OnGrappling(float InGrapplingValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappling);
}

void UPlayerBallStateIdle::OnGrappled(float InGrappledValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappled);
}

void UPlayerBallStateIdle::OnSnapped(float InSnapValue)
{
	if (StateMachine == nullptr || InSnapValue == 0.f)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Snapping);
}

void UPlayerBallStateIdle::OnDeath(float DeathValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Death);
}

void UPlayerBallStateIdle::OnRail(float RailDirectionValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Rail, RailDirectionValue);
}

void UPlayerBallStateIdle::OnUsePowerUp(float InPowerUpId)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::PowerUpHub, InPowerUpId);
}

