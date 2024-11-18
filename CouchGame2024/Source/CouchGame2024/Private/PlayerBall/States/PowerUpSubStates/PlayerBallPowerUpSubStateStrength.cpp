// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PowerUpSubStates/PlayerBallPowerUpSubStateStrength.h"

#include "PlayerBall/PlayerBallStateMachine.h"

EPlayerBallStateID UPlayerBallPowerUpSubStateStrength::GetStateID() const
{
	return EPlayerBallStateID::Strength;
}

void UPlayerBallPowerUpSubStateStrength::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallPowerUpSubStateStrength::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	
}

void UPlayerBallPowerUpSubStateStrength::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);


	
	if (StateMachine != nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallPowerUpSubStateStrength::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	
}
