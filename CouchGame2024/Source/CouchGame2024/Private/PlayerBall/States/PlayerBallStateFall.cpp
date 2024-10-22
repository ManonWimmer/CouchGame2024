// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateFall.h"

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

	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Fall")
	);

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateFall::OnStunned);
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateFall::OnPunch);
	}
}

void UPlayerBallStateFall::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateFall::OnStunned);
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateFall::OnPunch);
	}
}

void UPlayerBallStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	FallingMove(DeltaTime);

	CheckStillFalling();
}

void UPlayerBallStateFall::FallingMove(float DeltaTime)
{
	if (Pawn == nullptr)	return;

	if (Pawn->PawnMovement == nullptr)	return;

	FVector RightVect(0.f, 1.f, 0.f);

	FVector Dir = RightVect * Pawn->MoveXValue;	// Get ball Side dir

	Pawn->PawnMovement->AddInputVector(Dir);	// Move ball in air
}

void UPlayerBallStateFall::CheckStillFalling()
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->IsGrounded())	// IsGrounded -> Idle
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateFall::OnStunned(float StunnedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun);
}

void UPlayerBallStateFall::OnPunch(float PunchValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Punch);
}

