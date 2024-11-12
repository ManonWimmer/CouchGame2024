// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/PlayerBallStateMachine.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallState.h"
#include "PlayerBall/States/PlayerBallStateBumped.h"
#include "PlayerBall/States/PlayerBallStateDeath.h"
#include "PlayerBall/States/PlayerBallStateFall.h"
#include "PlayerBall/States/PlayerBallStateGrappled.h"
#include "PlayerBall/States/PlayerBallStateGrappling.h"
#include "PlayerBall/States/PlayerBallStateIdle.h"
#include "PlayerBall/States/PlayerBallStateImpact.h"
#include "PlayerBall/States/PlayerBallStateLocked.h"
#include "PlayerBall/States/PlayerBallStateMove.h"
#include "PlayerBall/States/PlayerBallStatePunch.h"
#include "PlayerBall/States/PlayerBallStateSnapping.h"
#include "PlayerBall/States/PlayerBallStateStun.h"

void UPlayerBallStateMachine::Init(APlayerBall* InPawn)
{
	Pawn = InPawn;
	
	//FindStates();

	CreateObjectStates();

	InitStates();

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, TEXT("Idle set"));
	ChangeState(EPlayerBallStateID::Idle);
}

APlayerBall* UPlayerBallStateMachine::GetPawn() const
{
	return Pawn;
}

void UPlayerBallStateMachine::ChangeState(EPlayerBallStateID NextStateID, float InFloatParameter)
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
		CurrentState->StateEnter(PreviousStateID, InFloatParameter);
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

void UPlayerBallStateMachine::CreateObjectStates()	// Create state as UObject and register them
{
	if (Pawn == nullptr)	return;

	TArray<EPlayerBallStateID> States = Pawn->PlayerStates;

	for (EPlayerBallStateID State : States)
	{
		CreateStateByID(State);
	}
}

void UPlayerBallStateMachine::CreateStateByID(EPlayerBallStateID InStateID)
{
	TObjectPtr<UPlayerBallState> OutState = nullptr;
	
	switch (InStateID)
	{
		case EPlayerBallStateID::Idle:
			OutState = NewObject<UPlayerBallStateIdle>(this);
			break;
		case EPlayerBallStateID::Move:
			OutState = NewObject<UPlayerBallStateMove>(this);
			break;
		case EPlayerBallStateID::Stun:
			OutState = NewObject<UPlayerBallStateStun>(this);
			break;
		case EPlayerBallStateID::Punch:
			OutState = NewObject<UPlayerBallStatePunch>(this);
			break;
		case EPlayerBallStateID::Impact:
			OutState = NewObject<UPlayerBallStateImpact>(this);
			break;
		case EPlayerBallStateID::Bumped:
			OutState = NewObject<UPlayerBallStateBumped>(this);
			break;
		case EPlayerBallStateID::Grappling:
			OutState = NewObject<UPlayerBallStateGrappling>(this);
			break;
		case EPlayerBallStateID::Grappled:
			OutState = NewObject<UPlayerBallStateGrappled>(this);
			break;
		case EPlayerBallStateID::Snapping:
			OutState = NewObject<UPlayerBallStateSnapping>(this);
			break;
		case EPlayerBallStateID::Locked:
			OutState = NewObject<UPlayerBallStateLocked>(this);
			break;
		case EPlayerBallStateID::Death:
			OutState = NewObject<UPlayerBallStateDeath>(this);
			break;
		default:
			break;
	}
	
	if (OutState != nullptr)
	{
		AllStates.Add(OutState);
	}
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
