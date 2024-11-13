// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateDeath.h"

#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"

EPlayerBallStateID UPlayerBallStateDeath::GetStateID() const
{
	return EPlayerBallStateID::Death;
}

void UPlayerBallStateDeath::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateDeath::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState : Death");

	if (Pawn != nullptr)
	{
		Pawn->bIsDead = true;
		
		if (Pawn->BehaviorGrapple)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;
		}

		Pawn->OnRespawnAction.AddDynamic(this, &UPlayerBallStateDeath::OnRespawn);
	}

	DeathBall();
}

void UPlayerBallStateDeath::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnRespawnAction.RemoveDynamic(this, &UPlayerBallStateDeath::OnRespawn);
	}

	EndDeathBall();
}

void UPlayerBallStateDeath::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

void UPlayerBallStateDeath::DeathBall()
{
	if (Pawn == nullptr)	return;

	Pawn->ResetMovement();
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(false);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	Pawn->SetActorHiddenInGame(true);
}

void UPlayerBallStateDeath::EndDeathBall()
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(true);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	Pawn->SetActorHiddenInGame(false);
}

void UPlayerBallStateDeath::OnRespawn(float RespawnValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Respawn);
}
