// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PowerUpSubStates/PlayerBallPowerUpSubStateSlippery.h"

#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"

EPlayerBallStateID UPlayerBallPowerUpSubStateSlippery::GetStateID() const
{
	return EPlayerBallStateID::Slippery;
}

void UPlayerBallPowerUpSubStateSlippery::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallPowerUpSubStateSlippery::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Player SubState powerUp : Slippery"));

	
	UseSlipperyPowerUp();

	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void UPlayerBallPowerUpSubStateSlippery::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	
}

void UPlayerBallPowerUpSubStateSlippery::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	
}

void UPlayerBallPowerUpSubStateSlippery::UseSlipperyPowerUp()
{
	APlayerBall* NearestPlayerBall = GetNearestPlayerBallInSlipperyRadius();

	if (Pawn != nullptr)
	{
		Pawn->PlaySlipperyCastGamefeelEffectsBlueprint();
	}
	
	if (NearestPlayerBall == nullptr)	return;
	if (NearestPlayerBall->BehaviorPowerUp == nullptr)	return;
	
	NearestPlayerBall->BehaviorPowerUp->ReceiveSlipperyEffect();
}

APlayerBall* UPlayerBallPowerUpSubStateSlippery::GetNearestPlayerBallInSlipperyRadius()
{
	if (Pawn == nullptr)	return nullptr;
	if (Pawn->BehaviorPowerUp == nullptr)	return nullptr;
	
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
		FCollisionShape::MakeSphere(Pawn->BehaviorPowerUp->SlipperyRadius),
		CollisionParams
	);
	
	if (Pawn->GetWorld())
		DrawDebugSphere(Pawn->GetWorld(), Start, Pawn->BehaviorPowerUp->SlipperyRadius, 12, FColor::Yellow, false, 3.f);

	if (bHasDetected)	// has detected a pawn
	{
		for (FOverlapResult& Result : OverlapResults)
		{
			APlayerBall* DetectedBall = Cast<APlayerBall>(Result.GetActor());	// Check if Player Ball
			if (DetectedBall)
			{
				return DetectedBall;
			}
		}
	}
	return nullptr;
}
