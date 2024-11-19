// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStatePunch.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"


// Sets default values for this component's properties
UPlayerBallStatePunch::UPlayerBallStatePunch()
{
	
}

EPlayerBallStateID UPlayerBallStatePunch::GetStateID() const
{
	return EPlayerBallStateID::Punch;
}

void UPlayerBallStatePunch::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStatePunch::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("PlayerState : Punch"));

	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Punch") );
	
	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;
		}
	}
	
	PunchPlayerBall();

	if (StateMachine != nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStatePunch::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);
	
	StartPunchCooldown();
}

void UPlayerBallStatePunch::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

#pragma region Punch Behavior
void UPlayerBallStatePunch::PunchPlayerBall()
{
	if (Pawn == nullptr)	return;
	
	APlayerBall* PlayerBall = GetNearestPlayerBallInPunchRadius();
	if (PlayerBall == nullptr)
		return;

	FVector Start = Pawn->GetActorLocation();
	FVector End = PlayerBall->GetActorLocation();

	FVector Dir = End - Start;

	Dir.Normalize();

	if (PlayerBall->BehaviorElementReactions != nullptr && Pawn->BehaviorElementReactions != nullptr)
	{
		PlayerBall->BehaviorElementReactions->ReceiveStunnedAction(Pawn->BehaviorElementReactions->PunchStunCooldown);
	
	}
	PlayerBall->SphereCollision->AddImpulse(Dir * Pawn->PunchForceMultiplier, NAME_None, false);
	
	Pawn->PlayPunchGamefeelEffects();
}

APlayerBall* UPlayerBallStatePunch::GetNearestPlayerBallInPunchRadius()
{
	if (Pawn == nullptr)
		return nullptr;
	
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
		FCollisionShape::MakeSphere(Pawn->PunchRadius),
		CollisionParams
	);
	
	if (Pawn->GetWorld())
		DrawDebugSphere(Pawn->GetWorld(), Start, Pawn->PunchRadius, 12, FColor::Blue, false, 3.f);

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

void UPlayerBallStatePunch::StartPunchCooldown()	// Setup playerBall parameters to handle PunchCooldown
{
	if (Pawn == nullptr)	return;

	Pawn->bCanPunch = false;
	Pawn->CurrentPunchCooldown = Pawn->PunchCooldown;
}

#pragma endregion 

void UPlayerBallStatePunch::OnImpacted(float ImpactedValue)	// -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStatePunch::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}


