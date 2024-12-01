// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateTourniquet.h"

#include "Components/SphereComponent.h"
#include "PinballElements/Elements/TourniquetElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"

EPlayerBallStateID UPlayerBallStateTourniquet::GetStateID() const
{
	return EPlayerBallStateID::Tourniquet;
}

void UPlayerBallStateTourniquet::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateTourniquet::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnEndTourniquetReaction.AddDynamic(this, &UPlayerBallStateTourniquet::ExitTourniquet);
		}

		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateTourniquet::OnForceExitTourniquet);
	}

	
	EnterTourniquet();
}

void UPlayerBallStateTourniquet::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
    {
    	if (Pawn->BehaviorElementReactions != nullptr)
    	{
    		Pawn->BehaviorElementReactions->OnEndTourniquetReaction.RemoveDynamic(this, &UPlayerBallStateTourniquet::ExitTourniquet);
    		
    		Pawn->BehaviorElementReactions->TourniquetElement = nullptr;
    	}

		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateTourniquet::OnForceExitTourniquet);
    }
}

void UPlayerBallStateTourniquet::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
}

void UPlayerBallStateTourniquet::EnterTourniquet()
{
	if (Pawn == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;

	
	Pawn->ResetMovement();

	Pawn->SphereCollision->SetSimulatePhysics(false);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void UPlayerBallStateTourniquet::ExitTourniquet(float ExitValues)
{
	if (Pawn == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;
	if (StateMachine == nullptr)	return;
	
	Pawn->SphereCollision->SetSimulatePhysics(true);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	Pawn->PlayReleaseTourniquetGamefeelEffectsBlueprint();
	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void UPlayerBallStateTourniquet::OnForceExitTourniquet(float InValue)
{
	if (Pawn == nullptr)	return;

	if (Pawn->BehaviorElementReactions == nullptr)	return;
	if (Pawn->BehaviorElementReactions->TourniquetElement == nullptr)	return;

	Pawn->BehaviorElementReactions->TourniquetElement->ForceExpulsePlayerFromTourniquetWithoutIndex(Pawn);
}
