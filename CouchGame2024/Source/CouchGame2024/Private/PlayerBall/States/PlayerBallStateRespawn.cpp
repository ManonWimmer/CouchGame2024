// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateRespawn.h"

#include "Components/SphereComponent.h"
#include "PinballElements/Elements/RailElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "Rounds/RoundsSubsystem.h"

EPlayerBallStateID UPlayerBallStateRespawn::GetStateID() const
{
	return EPlayerBallStateID::Respawn;
}

void UPlayerBallStateRespawn::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateRespawn::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState : Respawn");

	if (RespawnRailElement == nullptr)
	{
		InitRailRespawn();
	}

	
	if (Pawn != nullptr)
	{
		Pawn->bIsDead = false;
	}
	
	CurrentRespawnDuration = 0.f;

	RespawnBall();
}

void UPlayerBallStateRespawn::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);
	
	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->CurrentRailElement = RespawnRailElement;
		}
	}
	
	EndRespawnBall();
}

void UPlayerBallStateRespawn::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	HandleRespawnDuration(DeltaTime);
}

void UPlayerBallStateRespawn::RespawnBall()
{
	if (Pawn == nullptr)	return;

	if (RespawnRailElement != nullptr)
	{
		Pawn->SetActorLocation(RespawnRailElement->GetLocationAlongRailSpline(1.f));
	}
	
	Pawn->ResetMovement();
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(false);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	Pawn->PlayRespawnGamefeelEffectsBlueprint();
		
	Pawn->SetActorHiddenInGame(false);
}

void UPlayerBallStateRespawn::HandleRespawnDuration(float DeltaTime)
{
	if (CurrentRespawnDuration >= TotalRespawnDuration)
	{
		ExitRespawn();
	}
	else
	{
		CurrentRespawnDuration += DeltaTime;
	}
}

void UPlayerBallStateRespawn::ExitRespawn()
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Rail, 1.f);
}

void UPlayerBallStateRespawn::EndRespawnBall()
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(true);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	Pawn->SetActorHiddenInGame(false);
}

void UPlayerBallStateRespawn::InitRailRespawn()
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->GetWorld() == nullptr)	return;

	URoundsSubsystem* RoundsSubsystem = Pawn->GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr)	return;

	RespawnRailElement = RoundsSubsystem->GetRespawnRailElement();
}

