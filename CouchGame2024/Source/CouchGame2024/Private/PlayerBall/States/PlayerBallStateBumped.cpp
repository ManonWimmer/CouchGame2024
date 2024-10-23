// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateBumped.h"

#include "Components/SphereComponent.h"
#include "PinballElements/PinballElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateBumped::UPlayerBallStateBumped()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateBumped::GetStateID() const
{
	return EPlayerBallStateID::Bumped;
}

void UPlayerBallStateBumped::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateBumped::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Yellow,
		TEXT("PlayerState : Bumped")
	);
	
	CurrentHitLagTimer = Pawn->BumpedHitLagCooldown;
	
	Bump();

	if (Pawn != nullptr)
	{
		Pawn->HitPinballElement = nullptr;

		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateBumped::OnStunned);
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateBumped::OnImpacted);
		Pawn->OnBumperReaction.AddDynamic(this, &UPlayerBallStateBumped::OnBumped);
	}
}

void UPlayerBallStateBumped::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateBumped::OnStunned);
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateBumped::OnImpacted);
		Pawn->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateBumped::OnBumped);
	}
}

void UPlayerBallStateBumped::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateBumped::Bump()
{
	if (Pawn == nullptr)	return;

	if (Pawn->HitPinballElement == nullptr)	return;

	FVector Start = Pawn->HitPinballElement->GetActorLocation();
	FVector End = Pawn->GetActorLocation();

	FVector Dir = End - Start;

	Dir.Normalize();
	Pawn->SphereCollision->AddImpulse(Dir * Pawn->BumpedForceMultiplier, NAME_None, false);
}

void UPlayerBallStateBumped::DecreaseHitLagTimer(float DeltaTime)
{
	if (CurrentHitLagTimer > 0.f)
	{
		CurrentHitLagTimer -= DeltaTime;
	}
	else
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateBumped::OnStunned(float StunnedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun);
}

void UPlayerBallStateBumped::OnImpacted(float ImpactedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateBumped::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

