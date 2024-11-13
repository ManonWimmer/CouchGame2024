// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateRail.h"

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

	
}

void UPlayerBallStateRail::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	
}

void UPlayerBallStateRail::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
}
