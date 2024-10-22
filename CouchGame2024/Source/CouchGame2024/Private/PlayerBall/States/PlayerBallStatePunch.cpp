// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStatePunch.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStatePunch::UPlayerBallStatePunch()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	/*
	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Red,
		TEXT("PlayerState : Punch")
	);
	*/
	
	if (Pawn != nullptr)
	{
		CurrentPunchTimeRemaining = Pawn->PunchCooldown;
		
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStatePunch::OnImpacted);
	}
	
	PunchPlayerBall();
}

void UPlayerBallStatePunch::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStatePunch::OnImpacted);
	}
}

void UPlayerBallStatePunch::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	DecreaseCooldownPunch(DeltaTime);

	if (Pawn != nullptr)
	{
		if (Pawn->IsGrounded())
		{
			Move(DeltaTime);
		}
		else
		{
			FallingMove(DeltaTime);
		}
	}
}

void UPlayerBallStatePunch::PunchPlayerBall()
{
	APlayerBall* PlayerBall = GetNearestPlayerBallInPunchRadius();
	if (PlayerBall == nullptr)
		return;

	FString namePawn = Pawn->GetName();
	
	GEngine->AddOnScreenDebugMessage
	(
		-1,
		2.f,
		FColor::Green,
		namePawn
	);
	
	FVector Start = Pawn->GetActorLocation();
	FVector End = PlayerBall->GetActorLocation();

	FVector Dir = End - Start;

	Dir.Normalize();
	
	PlayerBall->ReceiveStunnedAction(1.f);
	PlayerBall->SphereCollision->AddImpulse(Dir * Pawn->PunchForceMultiplier, NAME_None, false);
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

	// Effectuer la détection de collision avec une sphère
	bool bHasDetected = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Start,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(Pawn->PunchRadius),
		CollisionParams
	);
	
	DrawDebugSphere(GetWorld(), Start, Pawn->PunchRadius, 12, FColor::Blue, false, 3.f);

	if (bHasDetected)	// has detected a pawn
	{
		for (FOverlapResult& Result : OverlapResults)
		{
			APlayerBall* DetectedBall = Cast<APlayerBall>(Result.GetActor());	// Check if Player Ball
			if (DetectedBall)
			{
				UE_LOG(LogTemp, Warning, TEXT("Ball Found : %s"), *DetectedBall->GetName());

				return DetectedBall;
			}
		}
	}
	return nullptr;
}

void UPlayerBallStatePunch::DecreaseCooldownPunch(float DeltaTime)
{
	if (CurrentPunchTimeRemaining > 0.f)
	{
		CurrentPunchTimeRemaining -= DeltaTime;
	}
	else
	{
		if (StateMachine != nullptr)
		{
			StateMachine->ChangeState(EPlayerBallStateID::Idle);
		}
	}
}

void UPlayerBallStatePunch::Move(float DeltaTime)	// roll ball X
{
	if (Pawn->PawnMovement == nullptr)
		return;

	FVector FwdVect = Pawn->GetActorForwardVector();

	FVector Dir = FwdVect * Pawn->MoveXValue;	// Get ball roll dir
	
	if (Pawn->SphereCollision == nullptr)
		return;

	bool SameDirection = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X <= 0 && Dir.X >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X >= 0 && Dir.X <= 0);

	if (SameDirection)	// same direction -> normal roll
	{
		Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -Pawn->AngularRollForce, NAME_None, true);	// Roll ball
	}
	else  // not same direction -> boost roll to braque faster
	{
		Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -(Pawn->AngularRollForce * Pawn->BraqueDirectionForceMultiplier), NAME_None, true);	// Roll ball
	}
}

void UPlayerBallStatePunch::FallingMove(float DeltaTime)	// AirControl fall X
{
	if (Pawn == nullptr)	return;

	if (Pawn->PawnMovement == nullptr)	return;

	FVector RightVect(0.f, 1.f, 0.f);

	FVector Dir = RightVect * Pawn->MoveXValue;	// Get ball Side dir

	Pawn->PawnMovement->AddInputVector(Dir);	// Move ball in air
}

void UPlayerBallStatePunch::OnImpacted(float ImpactedValue)	// -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}


