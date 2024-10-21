// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateMove.h"

#include "Components/SphereComponent.h"
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

	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Move")
	);
}

void UPlayerBallStateMove::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);
}

void UPlayerBallStateMove::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Move(DeltaTime);
	
	CheckNotMoving();
}

void UPlayerBallStateMove::Move(float DeltaTime)	// Move ball on X Axis by rolling it
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
		Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * GetWorld()->GetDeltaSeconds() * -Pawn->AngularRollForce, NAME_None, true);	// Roll ball
	}
	else  // not same direction -> boost roll to braque faster
	{
		Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * GetWorld()->GetDeltaSeconds() * -(Pawn->AngularRollForce * Pawn->BraqueDirectionForceMultiplier), NAME_None, true);	// Roll ball
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

