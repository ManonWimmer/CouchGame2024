// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateGrappled.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateGrappled::UPlayerBallStateGrappled()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateGrappled::GetStateID() const
{
	return EPlayerBallStateID::Grappled;
}

void UPlayerBallStateGrappled::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateGrappled::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1,2.f, FColor::Blue, TEXT("PlayerState : Grappled"));
	
	if (Pawn != nullptr)	// link state to events
	{
		Pawn->CanGrappling = false;
		
		Pawn->OnGrappledActionEnded.AddDynamic(this, &UPlayerBallStateGrappled::OnEndGrappled);
		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateGrappled::OnStunned);
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateGrappled::OnImpacted);
	}
}

void UPlayerBallStateGrappled::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
    {
		Pawn->CanBeGrappled = false;
		
    	Pawn->OnGrappledActionEnded.RemoveDynamic(this, &UPlayerBallStateGrappled::OnEndGrappled);
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateGrappled::OnStunned);
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateGrappled::OnImpacted);


		if (Pawn->GrapplingPlayerBall != nullptr)
		{
			Pawn->GrapplingPlayerBall->ReceiveGrapplingActionEnded(0.f);
			Pawn->GrapplingPlayerBall = nullptr;
		}
    }
}

void UPlayerBallStateGrappled::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateGrappled::OnEndGrappled(float InGrappledValue)	// receive event endGrappled -> Idle
{
	if (StateMachine != nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateGrappled::OnStunned(float StunnedValue)	// hit by punch -> stunned
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateGrappled::OnImpacted(float ImpactedValue)	// impact ball -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

