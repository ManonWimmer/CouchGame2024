// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateRail.h"

#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
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
		SpawnProgressRailDuration = CurrentRailElement->GetRailProgressDuration();

		CurrentRailSpeedMultiplier = CurrentRailElement->RailSpeedMultiplier;

		if (CurrentRailElement->SplineRail != nullptr)
		{
			CurrentSplineDistance = CurrentRailElement->SplineRail->GetSplineLength();
				
			InitProgressRailDurationDistance();
		}
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

	if (bOnRespawnRail)
	{
		HandleRailProgressLocationByPercent(DeltaTime);
	}
	else
	{
		//HandleRailProgressLocationByVelocity(DeltaTime);
		HandleRailProgressLocationByVelocityAndDistance(DeltaTime);
	}
}

void UPlayerBallStateRail::EnterRail()
{
	if (Pawn == nullptr)	return;
	
	CurrentPercentVelocityFromTarget = Pawn->GetVelocity().Length() / RefEnteringVelocity;

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Velocity when entering rails = : -> %s"), *Pawn->GetVelocity().ToString()));
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Percent Velocity when entering rails = : -> %f"), CurrentPercentVelocityFromTarget));


	if (CurrentRailElement != nullptr)
	{
		bOnRespawnRail = CurrentRailElement->ActorHasTag("RespawnRail");
	}
	
	Pawn->ResetMovement();
	
	if (Pawn->SphereCollision == nullptr)	return;

	Pawn->SphereCollision->SetSimulatePhysics(false);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

	Pawn->PlayRailEnterGamefeelEffectsBlueprint();
}

void UPlayerBallStateRail::HandleRailProgressLocationByPercent(float DeltaTime)	// Method ignoring entering velocity -> for respawn
{
	if (Pawn == nullptr)	return;
	if (CurrentRailElement == nullptr)	return;

	if (CurrentTimeInRail - EndProgressOffset >= SpawnProgressRailDuration)
	{
		ExitRail();
		return;
	}
	else
	{
		LastPos = Pawn->GetActorLocation();
		
		CheckForwardCollisionBallRail();
		
		CurrentTimeInRail += DeltaTime;

		CurrentPercent = CurrentTimeInRail / SpawnProgressRailDuration;

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

void UPlayerBallStateRail::HandleRailProgressLocationByVelocity(float DeltaTime)	// Method using entering velocity	-> for classic rail
{
	if (Pawn == nullptr)	return;
	if (CurrentRailElement == nullptr)	return;

	if (CurrentTimeInRail - EndProgressOffset >= ProgressRailDuration)
	{
		ExitRail();
		return;
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
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 25.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
		else
		{
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(CurrentPercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 25.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
	}
}

void UPlayerBallStateRail::HandleRailProgressLocationByVelocityAndDistance(float DeltaTime)
{
	if (Pawn == nullptr)	return;
	if (CurrentRailElement == nullptr)	return;

	if (CurrentTimeInRail - EndProgressOffset >= ProgressRailDurationDistance)
	{
		ExitRail();
		return;
	}
	else
	{
		LastPos = Pawn->GetActorLocation();
		
		CheckForwardCollisionBallRail(true);

		CurrentPercentVelocityFromTarget = FMath::FInterpTo(CurrentPercentVelocityFromTarget, 1.f, DeltaTime, AccelerationVelocity);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("New Percent Velocity when entering rails = : -> %f"), CurrentPercentVelocityFromTarget));

		
		CurrentTimeInRail += DeltaTime * CurrentPercentVelocityFromTarget * CurrentRailSpeedMultiplier;

		CurrentPercent = CurrentTimeInRail / ProgressRailDurationDistance;

		if (DirectionRail >= 0.f)
		{
			float InversePercent = 1.f - CurrentPercent;
		
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(InversePercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();
			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 25.f);
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
		else
		{
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(CurrentPercent);

			FVector NewPawnLocationOnRail = Pawn->GetActorLocation();

			NewPawnLocationOnRail = FMath::VInterpTo(NewPawnLocationOnRail, LocationAlongSpline, DeltaTime, 25.f);
			
		
			Pawn->SetActorLocation(NewPawnLocationOnRail);
		}
	}
}

void UPlayerBallStateRail::ExitRail()
{
	if (Pawn == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;
	if (StateMachine == nullptr)	return;

	if (!bOnRespawnRail)
	{
		if (DirectionRail >= 0.f)
		{
			float InversePercent = 1.f - CurrentPercent;
			
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(InversePercent);
			Pawn->SetActorLocation(LocationAlongSpline);
		}
		else
		{
			FVector LocationAlongSpline = CurrentRailElement->GetLocationAlongRailSpline(CurrentPercent);

			Pawn->SetActorLocation(LocationAlongSpline);
		}
	}
	
	
	Pawn->SphereCollision->SetSimulatePhysics(true);
	Pawn->SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	ExitImpulse();
	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);

	Pawn->PlayRailExitGamefeelEffectsBlueprint();
}

void UPlayerBallStateRail::ExitImpulse()
{
	if (Pawn == nullptr)	return;
	
	if (Pawn->SphereCollision == nullptr)	return;

	if (CurrentRailElement == nullptr)	return;
	
	FVector Dir = Pawn->GetActorLocation() - LastPos;
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Rail Impulse dir : %s"), *Dir.ToString()));

	Dir.Normalize();

	float ImpulseForce = 30000.f;

	if (CurrentRailElement != nullptr)
	{
		ImpulseForce = FMath::Lerp(CurrentRailElement->RailExpulseMinForce, CurrentRailElement->RailExpulseMaxForce, CurrentPercentVelocityFromTarget);
	}
	
	Pawn->SphereCollision->AddImpulse(Dir * ImpulseForce, NAME_None, false);	// impulse

	//DrawDebugLine(Pawn->GetWorld(), Pawn->GetActorLocation(), Pawn->GetActorLocation() + Dir * 1500.f, FColor::Yellow, false, 3.f);
}

void UPlayerBallStateRail::ChangeDirection()
{
	if (bOnRespawnRail)	return;
	
	DirectionRail *= -1;

	//CurrentTimeInRail = 1.f - CurrentTimeInRail;
	CurrentTimeInRail = ProgressRailDurationDistance - CurrentTimeInRail;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Silver, "Direction Rail CHANGING");
}

void UPlayerBallStateRail::CheckForwardCollisionBallRail(bool UseDistance)
{
	FVector CheckPosition = Pawn->GetActorLocation();

	float CurrentCheckPercent = 0.f;
	if (UseDistance)
	{
		CurrentCheckPercent = (CurrentTimeInRail + GetWorld()->GetDeltaSeconds()) / ProgressRailDurationDistance;
	}
	else
	{
		CurrentCheckPercent = (CurrentTimeInRail + GetWorld()->GetDeltaSeconds()) / ProgressRailDuration;
	}

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
		FCollisionShape::MakeSphere(Pawn->SphereCollision->GetScaledSphereRadius()/1.f),
		CollisionParams
	);

	//if (Pawn->GetWorld())
	//	DrawDebugSphere(Pawn->GetWorld(), CheckPosition, Pawn->SphereCollision->GetScaledSphereRadius()/1.f, 12, FColor::Yellow, false, 1.f);

	
	if (bHasDetected)
	{
		if (OverlapResults.Num() <= 0)	return;
		if (OverlapResults[0].GetActor() == nullptr)	return;

		if (!CheckGoingTowardLocation(OverlapResults[0].GetActor()->GetActorLocation(), CheckPosition))	return;
		
		ChangeDirection();
		
	}
}

bool UPlayerBallStateRail::CheckGoingTowardLocation(FVector OtherLocation, FVector TargetDirection)
{
	if (Pawn == nullptr)	return false;

	FVector PawnLocation = Pawn->GetActorLocation();
	FVector PawnToTargetPosition = TargetDirection - PawnLocation;
	FVector PawnToOtherPawn = OtherLocation - PawnLocation;
	
	if (FVector::DotProduct(PawnToTargetPosition, PawnToOtherPawn) < 0.f)	return false;

	return true;
}

void UPlayerBallStateRail::InitProgressRailDurationDistance()
{
	if (CurrentRailElement == nullptr)	return;

	ProgressRailDurationDistance = CurrentSplineDistance / RefSplineDistance;
}
