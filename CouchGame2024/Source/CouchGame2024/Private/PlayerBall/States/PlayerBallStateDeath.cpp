// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateDeath.h"

EPlayerBallStateID UPlayerBallStateDeath::GetStateID() const
{
	return EPlayerBallStateID::Death;
}

void UPlayerBallStateDeath::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateDeath::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState : Death");
	
}

void UPlayerBallStateDeath::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);


	
}

void UPlayerBallStateDeath::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateDeath::DeathBall()
{
	
}

void UPlayerBallStateDeath::UnDeathBall()
{
	
}
