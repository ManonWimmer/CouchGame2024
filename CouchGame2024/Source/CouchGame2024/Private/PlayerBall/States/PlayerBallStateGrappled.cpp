// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateGrappled.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"


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
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;

			Pawn->BehaviorGrapple->OnGrappledActionEnded.AddDynamic(this, &UPlayerBallStateGrappled::OnEndGrappled);
		}
		

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateGrappled::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateGrappled::OnImpacted);
		}
	}
}

void UPlayerBallStateGrappled::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
    {
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateGrappled::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateGrappled::OnImpacted);
		}

		
		if (Pawn->BehaviorGrapple != nullptr)
		{
    		Pawn->BehaviorGrapple->OnGrappledActionEnded.RemoveDynamic(this, &UPlayerBallStateGrappled::OnEndGrappled);


			
			if (Pawn->BehaviorGrapple->GrapplingPlayerBall != nullptr)
			{
				if (Pawn->BehaviorGrapple->GrapplingPlayerBall->BehaviorGrapple != nullptr)
				{
					Pawn->BehaviorGrapple->GrapplingPlayerBall->BehaviorGrapple->ReceiveGrapplingActionEnded(0.f);
				}
				Pawn->BehaviorGrapple->GrapplingPlayerBall = nullptr;
				UE_LOG(LogTemp, Warning, TEXT("UnSet grapplingPlayerBall") );
			}
		}
    }
}

void UPlayerBallStateGrappled::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateGrappled::OnEndGrappled(float InGrappledValue)	// receive event endGrappled -> Idle
{
	if (StateMachine == nullptr)	return;
	
	//UE_LOG(LogTemp, Warning, TEXT("Stop by release input or by grappling cause in grappled") );
	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void UPlayerBallStateGrappled::OnStunned(float StunnedValue)	// hit by punch -> stunned
{
	if (StateMachine == nullptr)	return;

	//UE_LOG(LogTemp, Warning, TEXT("Stop by stunned in grappled") );
	
	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateGrappled::OnImpacted(float ImpactedValue)	// impact ball -> impacted
{
	if (StateMachine == nullptr)	return;

	//UE_LOG(LogTemp, Warning, TEXT("Stop by impact in grappled") );
	
	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

