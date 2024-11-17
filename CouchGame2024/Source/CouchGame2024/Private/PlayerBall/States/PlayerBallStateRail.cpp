// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateRail.h"

#include "Components/SphereComponent.h"
#include "PinballElements/Elements/RailElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"

EPlayerBallStateID UPlayerBallStateRail::GetStateID() const
{
	return EPlayerBallStateID::Rail;
}

void UPlayerBallStateRail::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateRail::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PlayerState : Rail");

	CurrentPercent = 0.f;
	CurrentTimeInRail = 0.f;
	
	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			CurrentRailElement = Pawn->BehaviorElementReactions->CurrentRailElement;
		}
	}
}

void UPlayerBallStateRail::StateEnter(EPlayerBallStateID PreviousState, float InFloatParameter)
{
	Super::StateEnter(PreviousState, InFloatParameter);


	DirectionRail = InFloatParameter;

	if (CurrentRailElement != nullptr)
	{
		ProgressRailDuration = CurrentRailElement->GetRailProgressDuration();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("PlayerState : Rail : -> %s"), *Pawn->GetVelocity().ToString()));
	
	EnterRail();

	if (CurrentRailElement == nullptr)
	{
		ExitRail();
	}
}

void UPlayerBallStateRail::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->CurrentRailElement = nullptr;
		}
	}
	
	
	CurrentRailElement = nullptr;
}

void UPlayerBallStateRail::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	//HandleRailProgressLocationByPercent(DeltaTime);
	HandleRailProgressLocationByVelocity(DeltaTime);
}

void UPlayerBallStateRail::EnterRail()
{
	if (Pawn == nullptr)	return;
	
	CurrentPercentVelocityFromTarget = Pawn->GetVelocity().Length() / RefEnteringVelocity;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Velocity when entering rails = : -> %s"), *Pawn->GetVelocity().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Percent Velocity when entering rails = : -> %f"), CurrentPercentVelocityFromTarget));

	
	Pawn->ResetMovement();
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(false);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void UPlayerBallStateRail::HandleRailProgressLocationByPercent(float DeltaTime)	// Method ignoring entering velocity
{
	if (Pawn == nullptr)	return;
	if (CurrentRailElement == nullptr)	return;

	if (CurrentTimeInRail - EndProgressOffset >= ProgressRailDuration)
	{
		ExitRail();
	}
	else
	{
		LastPos = Pawn->GetActorLocation();
		
		CheckForwardCollisionBallRail();
		
		CurrentTimeInRail += DeltaTime;

		CurrentPercent = CurrentTimeInRail / ProgressRailDuration;

		if (DirectionRail >= 0.f)
		{
			float InversePercent = 1.f - CurrentPercent;
		
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(InversePercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 10.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
		else
		{
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(CurrentPercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 10.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
	}
}

void UPlayerBallStateRail::HandleRailProgressLocationByVelocity(float DeltaTime)	// Method using entering velocity
{
	if (Pawn == nullptr)	return;
	if (CurrentRailElement == nullptr)	return;

	if (CurrentTimeInRail - EndProgressOffset >= ProgressRailDuration)
	{
		ExitRail();
	}
	else
	{
		LastPos = Pawn->GetActorLocation();
		
		CheckForwardCollisionBallRail();

		CurrentPercentVelocityFromTarget = FMath::FInterpTo(CurrentPercentVelocityFromTarget, 1.f, DeltaTime, AccelerationVelocity);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("New Percent Velocity when entering rails = : -> %f"), CurrentPercentVelocityFromTarget));

		
		CurrentTimeInRail += DeltaTime * CurrentPercentVelocityFromTarget;

		CurrentPercent = CurrentTimeInRail / ProgressRailDuration;

		if (DirectionRail >= 0.f)
		{
			float InversePercent = 1.f - CurrentPercent;
		
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(InversePercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 10.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
		else
		{
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(CurrentPercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 10.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
	}
}

void UPlayerBallStateRail::ExitRail()
{
	if (Pawn == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;
	if (StateMachine == nullptr)	return;
	
	Pawn->SphereCollision->SetSimulatePhysics(true);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	ExitImpulse();
	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void UPlayerBallStateRail::ExitImpulse()
{
	if (Pawn == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;

	if (CurrentRailElement == nullptr)	return;
	
	//FVector Dir = CurrentRailElement->GetTangentAtSplinePercent(FMath::Clamp(CurrentPercent, 0.01f, 0.99f));
	FVector Dir = Pawn->GetActorLocation() - LastPos;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Rail Impulse dir : %s"), *Dir.ToString()));

	Dir.Normalize();

	
	Pawn->SphereCollision->AddImpulse(Dir * 30000.f, NAME_None, false);	// impulse

	DrawDebugLine(Pawn->GetWorld(), Pawn->GetActorLocation(), Pawn->GetActorLocation() + Dir * 1500.f, FColor::Yellow, false, 3.f);
}

void UPlayerBallStateRail::ChangeDirection()
{
	DirectionRail *= -1;

	CurrentTimeInRail = 1.f - CurrentTimeInRail;
}

void UPlayerBallStateRail::CheckForwardCollisionBallRail()
{
	FVector CheckPosition = Pawn->GetActorLocation();

	float CurrentCheckPercent = (CurrentTimeInRail + GetWorld()->GetDeltaSeconds()) / ProgressRailDuration;

	if (DirectionRail >= 0.f)
	{
		CurrentCheckPercent = 1.f - CurrentCheckPercent;
	}
	
	if (GetWorld())
		CheckPosition = CurrentRailElement->GetLocationAlongRailSpline(CurrentCheckPercent);
	
	
	FVector Start = CheckPosition;
	
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
		FCollisionShape::MakeSphere(Pawn->SphereCollision->GetScaledSphereRadius()/2.f),
		CollisionParams
	);

	//if (Pawn->GetWorld())
	//	DrawDebugSphere(Pawn->GetWorld(), CheckPosition, Pawn->SphereCollision->GetScaledSphereRadius()/2.f, 12, FColor::Yellow, false, 1.f);

	
	if (bHasDetected)
	{
		if (OverlapResults.Num() > 0)
		{
			ChangeDirection();
		}
	}
}
