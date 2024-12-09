// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PowerUpSubStates/PlayerBallPowerUpSubStateFreeze.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"

EPlayerBallStateID UPlayerBallPowerUpSubStateFreeze::GetStateID() const
{
	return EPlayerBallStateID::Freeze;
}

void UPlayerBallPowerUpSubStateFreeze::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallPowerUpSubStateFreeze::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Player SubState powerUp : Freeze"));


	UseFreezePowerUp();


	if (StateMachine != nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallPowerUpSubStateFreeze::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	
}

void UPlayerBallPowerUpSubStateFreeze::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	
}

void UPlayerBallPowerUpSubStateFreeze::UseFreezePowerUp()
{
	if (Pawn != nullptr)
	{
		Pawn->PlayFreezeCastGamefeelEffectsBlueprint();
	}
	
	TArray<APlayerBall*> PlayersInFreeze = GetPlayersInFreezeRange();

	if (PlayersInFreeze.Num() <= 0)	return;

	for (APlayerBall* InFreeze : PlayersInFreeze)
	{
		if (InFreeze == nullptr)	continue;

		if (InFreeze->BehaviorElementReactions == nullptr)	continue;

	
		InFreeze->BehaviorElementReactions->ReceiveStunnedAction(3.f);	//id 3.f -> Freeze stun
	}

}

TArray<APlayerBall*> UPlayerBallPowerUpSubStateFreeze::GetPlayersInFreezeRange() const
{
	TArray<APlayerBall*> OutPlayersInFreeze;
	
	if (Pawn == nullptr) return OutPlayersInFreeze;
	if (Pawn->BehaviorPowerUp == nullptr) return OutPlayersInFreeze;
	
	FVector Start = Pawn->GetActorLocation();
	
	// Array of hit results
	TArray<FOverlapResult> OverlapResults;

	// Set parameters
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);
	
	FCollisionObjectQueryParams ObjectQueryParams(ECollisionChannel::ECC_Pawn);	// Look only for pawn

	// Detect Collision With sphere overlap
	bool bHasDetected = Pawn->GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Start,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(Pawn->BehaviorPowerUp->FreezeRange),
		CollisionParams
	);
	
	//if (Pawn->GetWorld())
		//DrawDebugSphere(Pawn->GetWorld(), Start, Pawn->BehaviorPowerUp->FreezeRange, 12, FColor::Cyan, false, 3.f);

	if (!bHasDetected)	return OutPlayersInFreeze;

	for (FOverlapResult InFreeze : OverlapResults)
	{
		if (InFreeze.GetActor() == nullptr)	continue;
		
		APlayerBall* BallInFreeze = Cast<APlayerBall>(InFreeze.GetActor());

		if (BallInFreeze == nullptr)	continue;
		OutPlayersInFreeze.Add(BallInFreeze);
	}
	return OutPlayersInFreeze;
}
