// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/MultiplayerStates/PlayerBallStateDisconnected.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Match/MainMenuGameMode.h"
#include "PinballElements/Elements/RailElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"

EPlayerBallStateID UPlayerBallStateDisconnected::GetStateID() const
{
	return EPlayerBallStateID::Disconnected;
}

void UPlayerBallStateDisconnected::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);

	InitRailRespawn();
}

void UPlayerBallStateDisconnected::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState : Disconnected");

	DisconnectBall();
}

void UPlayerBallStateDisconnected::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->CurrentRailElement = SpawnRailElement;
		}
	}
	
	ExitDisconnect();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState : Connected");
}

void UPlayerBallStateDisconnected::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	
}

void UPlayerBallStateDisconnected::DisconnectBall()
{
	if (Pawn == nullptr)	return;
	
	Pawn->bIsConnected = false;

	Pawn->ResetMovement();
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(false);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	Pawn->SetActorHiddenInGame(true);
}

void UPlayerBallStateDisconnected::ExitDisconnect()
{
	if (Pawn == nullptr)	return;

	Pawn->bIsConnected = true;

	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(true);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	if (SpawnRailElement != nullptr)
	{
		Pawn->SetActorLocation(SpawnRailElement->GetLocationAlongRailSpline(1.f));
	}
	
	Pawn->SetActorHiddenInGame(false);
}

void UPlayerBallStateDisconnected::InitRailRespawn()
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->GetWorld() == nullptr)	return;

	AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this);
	
	if (GameMode == nullptr)	return;

	AMainMenuGameMode* MenuGameMode = Cast<AMainMenuGameMode>(GameMode);;

	if (MenuGameMode == nullptr)	return;

	SpawnRailElement = MenuGameMode->GetConnectionRailElements(Pawn->PlayerIndex);
	
}
