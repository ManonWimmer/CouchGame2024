// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateGrappled.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateGrappled::UPlayerBallStateGrappled()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateGrappled::GetStateID() const
{
	return EPlayerBallStateID::Grappled;
}

void UPlayerBallStateGrappled::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateGrappled::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	if (Pawn != nullptr)
	{
		Pawn->OnGrappledAction.AddDynamic(this, &UPlayerBallStateGrappled::OnEndGrappled);
	}
}

void UPlayerBallStateGrappled::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
    	{
    		Pawn->OnGrappledAction.RemoveDynamic(this, &UPlayerBallStateGrappled::OnEndGrappled);
    	}
}

void UPlayerBallStateGrappled::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateGrappled::OnEndGrappled(float InGrappledValue)
{
	if (InGrappledValue == 0)
	{
		if (StateMachine != nullptr)
		{
			StateMachine->ChangeState(EPlayerBallStateID::Grappled);
		}
	}
}

