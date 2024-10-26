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
		Pawn->OnBumperReaction.AddDynamic(this, &UPlayerBallStateMove::OnBumped);
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
		Pawn->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateMove::OnBumped);
	}
}

void UPlayerBallStateMove::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Move(DeltaTime);
	
	CheckNotMoving();

	CheckFalling();
}

void UPlayerBallStateMove::Move(float DeltaTime)	// Move ball on X and Y Axis by rolling it
{
	if (Pawn->PawnMovement == nullptr)
		return;

	//FVector FwdVect = Pawn->GetActorForwardVector();
	FVector FwdVect(1.f, 0.f, 0.f);

	//FVector UpVect = Pawn->GetActorUpVector();
	FVector UpVect(0.f, -1.f, 0.f);
	
	FVector Dir = (FwdVect * Pawn->MoveXValue) + (UpVect * Pawn->MoveYValue);	// Get ball roll dir
	
	if (Pawn->SphereCollision == nullptr)
		return;

	bool SameDirectionX = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X <= 0 && Dir.X >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X >= 0 && Dir.X <= 0);
	bool SameDirectionY = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().Y <= 0 && Dir.Y >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().Y >= 0 && Dir.Y <= 0);

	if (SameDirectionX || SameDirectionY)	// same direction -> normal roll
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

	if (FMathf::Abs(Pawn->MoveXValue) < 0.1f && FMathf::Abs(Pawn->MoveYValue) < 0.1f)	// Not moving -> Idle
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

void UPlayerBallStateMove::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

