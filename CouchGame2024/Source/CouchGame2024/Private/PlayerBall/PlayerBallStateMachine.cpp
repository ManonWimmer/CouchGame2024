// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/PlayerBallStateMachine.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallState.h"

void UPlayerBallStateMachine::Init(APlayerBall* InPawn)
{
	Pawn = InPawn;
	
	FindStates();
	InitStates();

	ChangeState(EPlayerBallStateID::Idle);
}

APlayerBall* UPlayerBallStateMachine::GetPawn() const
{
	return Pawn;
}

void UPlayerBallStateMachine::ChangeState(EPlayerBallStateID NextStateID)
{
	UPlayerBallState* NextState = GetState(NextStateID);

	if (NextState == nullptr)	return;

	if (CurrentState != nullptr)
	{
		CurrentState->StateExit(NextStateID);
	}

	EPlayerBallStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NextState;

	if (CurrentState != nullptr)
	{
		CurrentState->StateEnter(PreviousStateID);
	}
}

UPlayerBallState* UPlayerBallStateMachine::GetState(EPlayerBallStateID StateID)
{
	for (UPlayerBallState* State : AllStates)
	{
		if (StateID == State->GetStateID())
		{
			return State;
		}
	}

	return nullptr;
}

void UPlayerBallStateMachine::Tick(float DeltaTime)
{
	if (CurrentState == nullptr)	return;

	CurrentState->StateTick(DeltaTime);
}

UPlayerBallState* UPlayerBallStateMachine::GetCurrentState() const
{
	return CurrentState;
}

EPlayerBallStateID UPlayerBallStateMachine::GetCurrentStateID() const
{
	return CurrentStateID;
}

void UPlayerBallStateMachine::FindStates()
{
	TArray<UActorComponent*> FoundComponents = Pawn->K2_GetComponentsByClass(UPlayerBallState::StaticClass());

	for (UActorComponent* Component : FoundComponents)
	{
		UPlayerBallState* State = Cast<UPlayerBallState>(Component);

		if (State == nullptr)	continue;

		if (State->GetStateID() == EPlayerBallStateID::None)	continue;
		
		AllStates.Add(State);
	}
}

void UPlayerBallStateMachine::InitStates()
{
	for (UPlayerBallState* State : AllStates)
	{
		State->StateInit(this);
	}
}
