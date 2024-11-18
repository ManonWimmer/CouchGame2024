// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStatePowerUpHub.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"

EPlayerBallStateID UPlayerBallStatePowerUpHub::GetStateID() const
{
	return EPlayerBallStateID::PowerUpHub;
}

void UPlayerBallStatePowerUpHub::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStatePowerUpHub::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	
}

void UPlayerBallStatePowerUpHub::StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter)
{
	Super::StateEnter(PreviousState, InFloatParameter);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("PlayerState : PowerUpHub id : %f"), InFloatParameter));

	if (InFloatParameter == 1.f)	// id 1 -> freeze
	{
		ConsumeCurrentPowerUpPlayerBall();
		
		SwitchToFreezePowerUp();
	}
	else if (InFloatParameter == 2.f)	// id 2 -> Strength
	{
		ConsumeCurrentPowerUpPlayerBall();
		
		SwitchToStrengthPowerUp();
	}
	else if (InFloatParameter == 3.f)	// id 2 -> Slippery
	{
		ConsumeCurrentPowerUpPlayerBall();
		
		SwitchToSlipperyPowerUp();
	}
	else
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
	

}

void UPlayerBallStatePowerUpHub::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	
}

void UPlayerBallStatePowerUpHub::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	
}

void UPlayerBallStatePowerUpHub::ConsumeCurrentPowerUpPlayerBall()
{
	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorPowerUp != nullptr)
		{
			Pawn->BehaviorPowerUp->EmptyCurrentPowerUpCarried();
		}
	}
}

void UPlayerBallStatePowerUpHub::SwitchToFreezePowerUp()
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Freeze);
}


void UPlayerBallStatePowerUpHub::SwitchToStrengthPowerUp()
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Strength);
}

void UPlayerBallStatePowerUpHub::SwitchToSlipperyPowerUp()
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Slippery);
}
