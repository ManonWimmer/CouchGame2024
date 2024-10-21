// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateIdle.h"

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

	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Idle")
	);
}

void UPlayerBallStateIdle::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);
}

void UPlayerBallStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	OnMoveXCheck();
}

void UPlayerBallStateIdle::OnMoveXCheck()	// Check if ball start moving
{
	if (Pawn == nullptr)	return;

	if (FMathf::Abs(Pawn->MoveXValue) >= 0.1f)	// moving -> Move
	{
		if (StateMachine == nullptr)	return;

		StateMachine->ChangeState(EPlayerBallStateID::Move);
	}
}

