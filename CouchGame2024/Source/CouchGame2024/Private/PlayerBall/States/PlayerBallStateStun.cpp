// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateStun.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateStun::UPlayerBallStateStun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Stun")
	);
	
	if (Pawn != nullptr)
	{
		CurrentStunRemaining = Pawn->StunCooldown;


		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateStun::OnImpacted);
	}
}

void UPlayerBallStateStun::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateStun::OnImpacted);
	}
}

void UPlayerBallStateStun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	DecreaseCooldownStun(DeltaTime);
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


