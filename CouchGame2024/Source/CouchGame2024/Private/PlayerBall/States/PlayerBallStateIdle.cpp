// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateIdle.h"

#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateIdle::UPlayerBallStateIdle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	/*
	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Idle")
	);
	*/
	

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateIdle::OnStunned);
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateIdle::OnPunch);
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateIdle::OnImpacted);
		Pawn->OnBumperReaction.AddDynamic(this, &UPlayerBallStateIdle::OnBumped);
		Pawn->OnGrapplingAction.AddDynamic(this, &UPlayerBallStateIdle::OnGrappling);
		Pawn->OnGrappledAction.AddDynamic(this, &UPlayerBallStateIdle::OnGrappled);

		Pawn->OnReceiveSnappingAction.AddDynamic(this, &UPlayerBallStateIdle::OnSnapped);
	}
}

void UPlayerBallStateIdle::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnStunned);
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnPunch);
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnImpacted);
		Pawn->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateIdle::OnBumped);
		Pawn->OnGrapplingAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnGrappling);
		Pawn->OnGrappledAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnGrappled);

		Pawn->OnReceiveSnappingAction.RemoveDynamic(this, &UPlayerBallStateIdle::OnSnapped);
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

	if (FMathf::Abs(Pawn->MoveXValue) >= 0.1f || FMathf::Abs(Pawn->MoveYValue) >= 0.1f)	// moving -> Move
	{
		if (StateMachine == nullptr)	return;

		StateMachine->ChangeState(EPlayerBallStateID::Move);
	}
}

void UPlayerBallStateIdle::CheckFalling()	// Check if ball falling
{
	if (Pawn == nullptr)	return;

	if (!Pawn->IsGrounded())	// not on ground -> falling
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

