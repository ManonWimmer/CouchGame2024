﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateGrappling.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateGrappling::UPlayerBallStateGrappling()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateGrappling::GetStateID() const
{
	return EPlayerBallStateID::Grappling;
}

void UPlayerBallStateGrappling::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateGrappling::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	if (Pawn != nullptr)
	{
		Pawn->OnGrapplingAction.AddDynamic(this, &UPlayerBallStateGrappling::OnEndGrappling);
	}
}

void UPlayerBallStateGrappling::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnGrapplingAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnEndGrappling);
	}
}

void UPlayerBallStateGrappling::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
}

void UPlayerBallStateGrappling::OnEndGrappling(float InGrapplingValue)
{
	if (StateMachine == nullptr)	return;

	if (InGrapplingValue == 0.0f)
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

