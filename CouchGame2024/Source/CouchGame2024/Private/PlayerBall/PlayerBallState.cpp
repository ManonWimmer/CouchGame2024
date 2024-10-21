// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/PlayerBallState.h"

#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallState::UPlayerBallState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	
}

void UPlayerBallState::StateExit(EPlayerBallStateID NextState)
{
	
}

void UPlayerBallState::StateTick(float DeltaTime)
{
	
}


