// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateFall.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateFall::UPlayerBallStateFall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateFall::GetStateID() const
{
	return EPlayerBallStateID::Fall;
}

void UPlayerBallStateFall::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateFall::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	/*
	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Fall")
	);
	*/

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateFall::OnStunned);
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateFall::OnPunch);
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateFall::OnImpacted);
		Pawn->OnBumperReaction.AddDynamic(this, &UPlayerBallStateFall::OnBumped);
	}
}

void UPlayerBallStateFall::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateFall::OnStunned);
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateFall::OnPunch);
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateFall::OnImpacted);
		Pawn->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateFall::OnBumped);
	}
}

void UPlayerBallStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	FallingMoveX(DeltaTime);
	FallingMoveY(DeltaTime);
	
	CheckStillFalling();
}

void UPlayerBallStateFall::FallingMoveX(float DeltaTime) const	// Air control X
{
	if (Pawn == nullptr)	return;

	if (Pawn->PawnMovement == nullptr)	return;

	FVector RightVect(0.f, 1.f, 0.f);

	FVector Dir = RightVect * Pawn->MoveXValue;	// Get ball Side dir

	Pawn->PawnMovement->AddInputVector(Dir);	// Move ball in air
}

void UPlayerBallStateFall::FallingMoveY(float DeltaTime) const	// Speed fall control Y
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->MoveYValue > 0.1f && !(Pawn->GetVelocity().Z > -50.f))
	{
		Pawn->SphereCollision->AddForce(FVector(0.f, 0.f, 1.f) * Pawn->SlowFallForce * Pawn->MoveYValue, NAME_None, true);
	}
	else if (Pawn->MoveYValue < -0.1f)
	{
		Pawn->SphereCollision->AddForce(FVector(0.f, 0.f, 1.f) * Pawn->AccelerateFallForce * Pawn->MoveYValue, NAME_None, true);
	}
}

void UPlayerBallStateFall::CheckStillFalling()	// check falling
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->IsGrounded())	// IsGrounded -> Idle
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateFall::OnStunned(float StunnedValue)	// -> stun
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun);
}

void UPlayerBallStateFall::OnPunch(float PunchValue)	// -> punch
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Punch);
}

void UPlayerBallStateFall::OnImpacted(float ImpactedValue)	// -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateFall::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

