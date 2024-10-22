// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateImpact.h"

#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


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

	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Impact")
	);

	ImpactedBall(1.f);

	Pawn->ImpactedPlayerBall = nullptr;
	
	if (Pawn != nullptr)
	{
		if (Pawn->ImpactedPlayerBall == nullptr)
		{
			StateMachine->ChangeState(EPlayerBallStateID::Idle);
		}
	}
}

void UPlayerBallStateImpact::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn == nullptr)
		return;

	Pawn->ImpactedPlayerBall = nullptr;
}

void UPlayerBallStateImpact::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateImpact::ImpactedBall(float ImpactValue)
{
	if (Pawn == nullptr)
		return;

	if (Pawn->ImpactedPlayerBall == nullptr)
		return;
	
	FVector Start = Pawn->ImpactedPlayerBall->GetActorLocation();
	FVector End = Pawn->GetActorLocation();

	FVector Dir = End - Start;

	Dir.Normalize();
	
	Pawn->ReceiveStunnedAction(1.f);
	Pawn->SphereCollision->AddImpulse(Dir * Pawn->ImpactForceMultiplier, NAME_None, false);
}


