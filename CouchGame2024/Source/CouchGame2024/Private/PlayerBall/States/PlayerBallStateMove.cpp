// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateMove.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateMove::UPlayerBallStateMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateMove::GetStateID() const
{
	return EPlayerBallStateID::Move;
}

void UPlayerBallStateMove::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateMove::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	/*
	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Move")
	);
	*/

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateMove::OnStunned);
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateMove::OnPunch);
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateMove::OnImpacted);
	}
}

void UPlayerBallStateMove::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateMove::OnStunned);
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateMove::OnPunch);
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateMove::OnImpacted);
	}
}

void UPlayerBallStateMove::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	MoveX(DeltaTime);
	
	CheckNotMoving();

	CheckFalling();
}

void UPlayerBallStateMove::MoveX(float DeltaTime)	// Move ball on X Axis by rolling it
{
	if (Pawn->PawnMovement == nullptr)
		return;

	FVector FwdVect = Pawn->GetActorForwardVector();

	FVector Dir = FwdVect * Pawn->MoveXValue;	// Get ball roll dir
	
	if (Pawn->SphereCollision == nullptr)
		return;

	bool SameDirection = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X <= 0 && Dir.X >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X >= 0 && Dir.X <= 0);

	if (SameDirection)	// same direction -> normal roll
	{
		Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -Pawn->AngularRollForce, NAME_None, true);	// Roll ball
	}
	else  // not same direction -> boost roll to braque faster
	{
		Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -(Pawn->AngularRollForce * Pawn->BraqueDirectionForceMultiplier), NAME_None, true);	// Roll ball
	}
}

void UPlayerBallStateMove::CheckNotMoving()	// Check if ball is still moving
{
	if (Pawn == nullptr)	return;

	if (FMathf::Abs(Pawn->MoveXValue) < 0.1f)	// Not moving -> Idle
	{
		if (StateMachine == nullptr)	return;

		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateMove::CheckFalling()	// check falling -> falling
{
	if (Pawn == nullptr)	return;

	if (!Pawn->IsGrounded())	// not on ground -> falling
	{
		StateMachine->ChangeState(EPlayerBallStateID::Fall);
	}
}

void UPlayerBallStateMove::OnStunned(float StunnedValue)	// -> stunned
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun);
}

void UPlayerBallStateMove::OnPunch(float PunchValue)	// -> punch
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Punch);
}

void UPlayerBallStateMove::OnImpacted(float ImpactedValue)	// -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

