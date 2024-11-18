// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateStun.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"


// Sets default values for this component's properties
UPlayerBallStateStun::UPlayerBallStateStun()
{
	
}

EPlayerBallStateID UPlayerBallStateStun::GetStateID() const
{
	return EPlayerBallStateID::Stun;
}

void UPlayerBallStateStun::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateStun::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("PlayerState : Stun"));

	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Stun") );
	
	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.AddDynamic(this, &UPlayerBallStateStun::OnDeath);

		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = true;

			Pawn->BehaviorGrapple->OnGrappledActionStarted.AddDynamic(this, &UPlayerBallStateStun::OnGrappled);
		}

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateStun::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.AddDynamic(this, &UPlayerBallStateStun::OnBumped);
			//Pawn->BehaviorElementReactions->OnReceiveSnappingAction.AddDynamic(this, &UPlayerBallStateStun::OnSnapped);

			Pawn->BehaviorElementReactions->OnRailReaction.AddDynamic(this, &UPlayerBallStateStun::OnRail);
		}
	}
}

void UPlayerBallStateStun::StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter)
{
	Super::StateEnter(PreviousState, InFloatParameter);

	if (InFloatParameter == 1.f)	// id 1.f -> bumped stun
	{
		CurrentStunRemaining = Pawn->BehaviorElementReactions->BumpedHitLagCooldown;
	}
	else if (InFloatParameter == 2.f)	//id 2.f -> Impact stun
	{
		CurrentStunRemaining = Pawn->BehaviorElementReactions->ImpactStunCooldown;
	}
	else if (InFloatParameter == 3.f)	//id 3.f -> Freeze stun
	{
		CurrentStunRemaining = Pawn->BehaviorPowerUp->FreezeDuration;
		FreezeStunVariant();
	}
	else
	{
		CurrentStunRemaining = InFloatParameter;
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("PlayerState : stun id : %f"), InFloatParameter));
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("PlayerState : stun duration : %f"), CurrentStunRemaining));
	

}

void UPlayerBallStateStun::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.RemoveDynamic(this, &UPlayerBallStateStun::OnDeath);

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateStun::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateStun::OnBumped);
			//Pawn->BehaviorElementReactions->OnReceiveSnappingAction.RemoveDynamic(this, &UPlayerBallStateStun::OnSnapped);

			Pawn->BehaviorElementReactions->OnRailReaction.RemoveDynamic(this, &UPlayerBallStateStun::OnRail);
		}
		
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->OnGrappledActionStarted.RemoveDynamic(this, &UPlayerBallStateStun::OnGrappled);
		}
	}
}

void UPlayerBallStateStun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	DecreaseCooldownStun(DeltaTime);
}


void UPlayerBallStateStun::FreezeStunVariant()
{
	if (Pawn == nullptr)	return;

	Pawn->ResetMovement();
}

void UPlayerBallStateStun::DecreaseCooldownStun(float DeltaTime)	// cooldown before exit stun -> Idle
{
	if (CurrentStunRemaining > 0.f)	// Stun cooldown not finish -> decrease
	{
		CurrentStunRemaining -= DeltaTime;
	}
	else
	{
		if (StateMachine != nullptr)	// Stun cooldown ended -> Idle
		{
			StateMachine->ChangeState(EPlayerBallStateID::Idle);
		} 
	}
}

void UPlayerBallStateStun::OnImpacted(float ImpactedValue)	// -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateStun::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

void UPlayerBallStateStun::OnSnapped(float InSnapValue)
{
	if (StateMachine == nullptr || InSnapValue == 0.f)	return;

	//StateMachine->ChangeState(EPlayerBallStateID::Snapping);
}

void UPlayerBallStateStun::OnGrappled(float GrappledValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappled);
}

void UPlayerBallStateStun::OnDeath(float DeathValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Death);
}

void UPlayerBallStateStun::OnRail(float RailDirectionValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Rail, RailDirectionValue);
}


