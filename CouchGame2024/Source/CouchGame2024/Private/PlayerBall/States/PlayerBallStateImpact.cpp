﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateImpact.h"

#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"


// Sets default values for this component's properties
UPlayerBallStateImpact::UPlayerBallStateImpact()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateImpact::GetStateID() const
{
	return EPlayerBallStateID::Impact;
}

void UPlayerBallStateImpact::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateImpact::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	/*
	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Impact")
	);
	*/

	if (Pawn != nullptr)
	{
		Pawn->CanGrappling = false;
		Pawn->CanBeGrappled = false;
	}
	
	ImpactedBall(1.f);

	if (Pawn->BehaviorElementReactions != nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Stun, Pawn->BehaviorElementReactions->ImpactStunCooldown);	// Stun
	}
	else
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateImpact::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn == nullptr)
		return;

	if (Pawn->BehaviorElementReactions == nullptr)
		return;
	
	Pawn->BehaviorElementReactions->ImpactedPlayerBall = nullptr;
}

void UPlayerBallStateImpact::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateImpact::ImpactedBall(float ImpactValue)	// bounce ball in opposite direction from impact point
{
	if (Pawn == nullptr)
		return;

	if (Pawn->BehaviorElementReactions == nullptr)
		return;
	
	if (Pawn->BehaviorElementReactions->ImpactedPlayerBall == nullptr)
		return;

	FVector Velocity = Pawn->BehaviorElementReactions->ImpactedPlayerBall->GetVelocity();
	FVector Start = Pawn->GetActorLocation();
	FVector End = Pawn->BehaviorElementReactions->ImpactedPlayerBall->GetActorLocation();

	FVector Dir = End - Start;

	Dir = FMath::GetReflectionVector(Dir, Pawn->BehaviorElementReactions->NormalImpact);
	
	Dir.Normalize();
	
	float TotalForce = Pawn->BehaviorElementReactions->ImpactForceMultiplier * Velocity.Length();

	if (TotalForce < Pawn->BehaviorElementReactions->ImpactMinTotalForce)
	{
		TotalForce = Pawn->BehaviorElementReactions->ImpactMinTotalForce;
	}

	Pawn->PlayImpactEffectsBlueprint();
	
	Pawn->SphereCollision->AddImpulse(Dir * TotalForce, NAME_None, false);	// impulse
}


