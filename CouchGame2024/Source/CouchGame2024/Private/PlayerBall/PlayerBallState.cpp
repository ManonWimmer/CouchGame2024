// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/PlayerBallState.h"

#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallState::UPlayerBallState()
{
	
}

EPlayerBallStateID UPlayerBallState::GetStateID() const
{
	return EPlayerBallStateID::None;
}

void UPlayerBallState::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;

	if (StateMachine != nullptr)
	{
		Pawn = StateMachine->GetPawn();
	}
}

void UPlayerBallState::StateEnter(EPlayerBallStateID PreviousState)
{
	ReceiveStateEnter(PreviousState);
}

void UPlayerBallState::StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter)
{
	StateEnter(GetStateID());
}

void UPlayerBallState::StateExit(EPlayerBallStateID NextState)
{
	
}

void UPlayerBallState::StateTick(float DeltaTime)
{
	
}


