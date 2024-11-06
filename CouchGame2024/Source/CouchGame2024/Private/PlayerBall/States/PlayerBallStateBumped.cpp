// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateBumped.h"

#include "Components/SphereComponent.h"
#include "PinballElements/PinballElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"


// Sets default values for this component's properties
UPlayerBallStateBumped::UPlayerBallStateBumped()
{
	
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

	/*
	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Yellow,
		TEXT("PlayerState : Bumped")
	);
	*/

	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Bumped") );

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;
		}

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateBumped::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateBumped::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.AddDynamic(this, &UPlayerBallStateBumped::OnBumped);
		}
	}
	
	Bump();
}

void UPlayerBallStateBumped::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->HitPinballElement = nullptr;
			
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateBumped::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateBumped::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateBumped::OnBumped);
		}
	}
}

void UPlayerBallStateBumped::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateBumped::Bump()
{
	if (Pawn == nullptr)	return;

	if (Pawn->BehaviorElementReactions == nullptr)	return;
	
	if (Pawn->BehaviorElementReactions->HitPinballElement == nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
		return;
	}

	FVector Start = Pawn->GetActorLocation();
	FVector End = Pawn->BehaviorElementReactions->HitPinballElement->GetActorLocation();

	FVector Dir = End - Start;

	Dir = FMath::GetReflectionVector(Dir, Pawn->BehaviorElementReactions->NormalBump);
	

	//DrawDebugLine(Pawn->GetWorld(), Pawn->HitPinballElement->GetActorLocation(), Pawn->HitPinballElement->GetActorLocation() + Dir*1000.f, FColor::Green, true, 5.f);
	Dir.Normalize();
	
	Pawn->SphereCollision->AddImpulse(Dir * Pawn->BehaviorElementReactions->BumpedForceMultiplier, NAME_None, false);	// impulse

	Pawn->BehaviorElementReactions->ReceiveStunnedAction(Pawn->BehaviorElementReactions->BumpedHitLagCooldown);	// stun
}

void UPlayerBallStateBumped::OnStunned(float StunnedValue)	// -> stunned
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateBumped::OnImpacted(float ImpactedValue)	// -> impact
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateBumped::OnBumped(float BumpedValue)	// -> bumped
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

