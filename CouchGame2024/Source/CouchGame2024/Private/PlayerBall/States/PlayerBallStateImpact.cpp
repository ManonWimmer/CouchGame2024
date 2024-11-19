﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateImpact.h"

#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"


// Sets default values for this component's properties
UPlayerBallStateImpact::UPlayerBallStateImpact()
{
	
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

	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Impact") );

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;
		}
	}
	
	ImpactedBall(1.f);

	if (Pawn->BehaviorElementReactions != nullptr)
	{
		float ImpactStunDurationDivider = 1.f;

		if (Pawn->BehaviorPowerUp != nullptr)
		{
			if (Pawn->BehaviorPowerUp->GetIsUsingStrengthPowerUp() && ImpactStunDurationDivider != 0.f)
			{
				ImpactStunDurationDivider = Pawn->BehaviorPowerUp->StrengthImpactStunDurationDivider;
			}
		}
		
		StateMachine->ChangeState(EPlayerBallStateID::Stun, 2.f / ImpactStunDurationDivider);	// Stun id 2.f -> ImpactStunCooldown
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

	float ImpactForceDivider = 1.f;
	
	if (Pawn->BehaviorPowerUp != nullptr)
	{
		if (Pawn->BehaviorPowerUp->GetIsUsingStrengthPowerUp() && Pawn->BehaviorPowerUp->StrengthImpactForceDivider != 0.f)
		{
			ImpactForceDivider = Pawn->BehaviorPowerUp->StrengthImpactForceDivider;
		}
	}
	
	Pawn->SphereCollision->AddImpulse(Dir * TotalForce / ImpactForceDivider, NAME_None, false);	// impulse

	Pawn->PlayImpactGamefeelEffectsBlueprint();
}


