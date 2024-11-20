// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PowerUpSubStates/PlayerBallPowerUpSubStateStrength.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"

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

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Player SubState powerUp : Strength"));


	UseStrengthPowerUp();

	if (StateMachine != nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallPowerUpSubStateStrength::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);


	
}

void UPlayerBallPowerUpSubStateStrength::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	
}


void UPlayerBallPowerUpSubStateStrength::UseStrengthPowerUp()
{
	if (Pawn == nullptr)	return;
	if (Pawn->BehaviorPowerUp == nullptr)	return;

	Pawn->BehaviorPowerUp->ActivateStrengthPowerUp();
}