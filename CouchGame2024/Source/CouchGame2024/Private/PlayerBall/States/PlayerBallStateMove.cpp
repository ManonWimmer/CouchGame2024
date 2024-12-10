// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateMove.h"

#include "Components/SphereComponent.h"
#include "PhysicsEngine/BodySetup.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"
#include "PlayerBall/Datas/PlayerBallData.h"


// Sets default values for this component's properties
UPlayerBallStateMove::UPlayerBallStateMove()
{
	
}

EPlayerBallStateID UPlayerBallStateMove::GetStateID() const
{
	return EPlayerBallStateID::Move;
}

void UPlayerBallStateMove::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateMove::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("PlayerState : Move"));

	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Move") );

	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.AddDynamic(this, &UPlayerBallStateMove::OnDeath);

		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = true;
			Pawn->BehaviorGrapple->CanBeGrappled = true;

			Pawn->BehaviorGrapple->OnGrapplingActionStarted.AddDynamic(this, &UPlayerBallStateMove::OnGrappling);
			Pawn->BehaviorGrapple->OnGrappledActionStarted.AddDynamic(this, &UPlayerBallStateMove::OnGrappled);
		}

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateMove::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateMove::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.AddDynamic(this, &UPlayerBallStateMove::OnBumped);
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.AddDynamic(this, &UPlayerBallStateMove::OnSnapped);

			Pawn->BehaviorElementReactions->OnRailReaction.AddDynamic(this, &UPlayerBallStateMove::OnRail);
			
			Pawn->BehaviorElementReactions->OnTourniquetReaction.AddDynamic(this, &UPlayerBallStateMove::OnTourniquet);

		}

		if (Pawn->BehaviorPowerUp != nullptr)
		{
			Pawn->BehaviorPowerUp->OnUsePowerUpAction.AddDynamic(this, &UPlayerBallStateMove::OnUsePowerUp);
		}
		
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateMove::OnPunch);
	}
}

void UPlayerBallStateMove::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.RemoveDynamic(this, &UPlayerBallStateMove::OnDeath);

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateMove::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateMove::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateMove::OnBumped);
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.RemoveDynamic(this, &UPlayerBallStateMove::OnSnapped);

			Pawn->BehaviorElementReactions->OnRailReaction.RemoveDynamic(this, &UPlayerBallStateMove::OnRail);

			Pawn->BehaviorElementReactions->OnTourniquetReaction.RemoveDynamic(this, &UPlayerBallStateMove::OnTourniquet);

		}

		if (Pawn->BehaviorPowerUp != nullptr)
		{
			Pawn->BehaviorPowerUp->OnUsePowerUpAction.RemoveDynamic(this, &UPlayerBallStateMove::OnUsePowerUp);
		}
		
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateMove::OnPunch);
		
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->OnGrapplingActionStarted.RemoveDynamic(this, &UPlayerBallStateMove::OnGrappling);
			Pawn->BehaviorGrapple->OnGrappledActionStarted.RemoveDynamic(this, &UPlayerBallStateMove::OnGrappled);
		}
	}

	SetRightMoveMaterial(false);
}

void UPlayerBallStateMove::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Move(DeltaTime);
	
	CheckNotMoving();

	CheckFalling();
}

void UPlayerBallStateMove::Move(float DeltaTime)	// Move ball on X and Y Axis by rolling it
{
	if (Pawn->BehaviorMovements == nullptr)	return;
	
	
	FVector FwdVect(1.f, 0.f, 0.f);
	FVector RightVect(0.f, -1.f, 0.f);
	FVector Dir = (FwdVect * Pawn->BehaviorMovements->MoveXValue) + (RightVect * Pawn->BehaviorMovements->MoveYValue);	// Get ball roll dir
	
	if (Pawn->SphereCollision == nullptr)
		return;

	/* OLD VERSION
	bool SameDirectionX = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X <= 0 && Dir.X >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X >= 0 && Dir.X <= 0);
	bool SameDirectionY = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().Y <= 0 && Dir.Y >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().Y >= 0 && Dir.Y <= 0);

	if (!SameDirectionX)	// May Increase roll X if oppositeDirection
	{
		Dir.X *= Pawn->BehaviorMovements->BraqueDirectionForceMultiplier;
	}
	if (!SameDirectionY)	// May Increase roll Y if oppositeDirection
	{
		Dir.Y *= Pawn->BehaviorMovements->BraqueDirectionForceMultiplier;
	}
	*/
	
	bool SameDirectionX = true;
	bool SameDirectionY = true;
	if (FMath::Abs(Dir.Y) > 0.2f)
	{
		SameDirectionX = (Pawn->GetVelocity().X * Dir.Y < -5.f); // Vérifie si les signes sont identiques
	}
	if (FMath::Abs(Dir.X) > 0.2f)
	{
		SameDirectionY = (Pawn->GetVelocity().Y * Dir.X > 5.f); // Vérifie si les signes sont identiques
	}
	
	if (!SameDirectionX)	// May Increase roll X if oppositeDirection
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Y: %f , Vel X : %f"), Dir.Y, Pawn->GetVelocity().X));
		Dir.Y *= Pawn->BehaviorMovements->BraqueDirectionForceMultiplier;
	}
	if (!SameDirectionY)	// May Increase roll Y if oppositeDirection
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("X: %f , Vel Y : %f"), Dir.X, Pawn->GetVelocity().Y));
		Dir.X *= Pawn->BehaviorMovements->BraqueDirectionForceMultiplier;
	}


	SetRightMoveMaterial(!SameDirectionX || !SameDirectionY);

	//DrawDebugLine(Pawn->GetWorld(), Pawn->GetActorLocation(), Pawn->GetActorLocation() + Dir * 500.f, FColor::Orange, false, 5.f);

	Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -Pawn->BehaviorMovements->GetContextRollForce(), NAME_None, true);	// Roll ball
}

void UPlayerBallStateMove::SetRightMoveMaterial(bool IsBrakeDirection)
{
	if (Pawn == nullptr)	return;
	if (Pawn->BehaviorPowerUp == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;
	if (Pawn->GetPlayerBallData() == nullptr)	return;
	if (Pawn->GetPlayerBallData()->BrakeDirectionPhysicsAsset == nullptr)	return;
	if (Pawn->GetPlayerBallData()->ClassicPhysicsAsset == nullptr)	return;

	if (Pawn->BehaviorPowerUp->GetIsSlippery())	return;
	
	if (IsBrakeDirection)
	{
		if (Pawn->bIsBrakeDirection)	return;
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Change material to brake direction direction"));

		Pawn->bIsBrakeDirection = true;
			
		Pawn->SphereCollision->SetPhysMaterialOverride(Pawn->GetPlayerBallData()->BrakeDirectionPhysicsAsset);
	}
	else
	{
		if (!Pawn->bIsBrakeDirection)	return;
		
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Change material to classic direction"));

		Pawn->bIsBrakeDirection = false;
			
		Pawn->SphereCollision->SetPhysMaterialOverride(Pawn->GetPlayerBallData()->ClassicPhysicsAsset);
	}
}

void UPlayerBallStateMove::CheckNotMoving()	// Check if ball is still moving
{
	if (Pawn == nullptr)	return;

	if (Pawn->BehaviorMovements == nullptr)	return;
	
	if (FMathf::Abs(Pawn->BehaviorMovements->MoveXValue) < 0.1f && FMathf::Abs(Pawn->BehaviorMovements->MoveYValue) < 0.1f)	// Not moving -> Idle
	{
		if (StateMachine == nullptr)	return;

		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateMove::CheckFalling()	// check falling -> falling
{
	if (Pawn == nullptr)	return;

	if (Pawn->BehaviorMovements == nullptr)	return;
	
	if (!Pawn->BehaviorMovements->IsGrounded())	// not on ground -> falling
	{
		StateMachine->ChangeState(EPlayerBallStateID::Fall);
	}
}

void UPlayerBallStateMove::OnStunned(float StunnedValue)	// -> stunned
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateMove::OnPunch(float PunchValue)	// -> punch
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Punch);
}

void UPlayerBallStateMove::OnImpacted(float ImpactedValue)	// -> impacted
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateMove::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

void UPlayerBallStateMove::OnGrappling(float InGrapplingValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappling);
}

void UPlayerBallStateMove::OnGrappled(float InGrappledValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappled);
}

void UPlayerBallStateMove::OnSnapped(float InSnapValue)
{
	if (StateMachine == nullptr || InSnapValue == 0.f)	return;
	
	StateMachine->ChangeState(EPlayerBallStateID::Snapping);
}

void UPlayerBallStateMove::OnDeath(float DeathValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Death);
}

void UPlayerBallStateMove::OnRail(float RailDirectionValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Rail, RailDirectionValue);
}

void UPlayerBallStateMove::OnUsePowerUp(float InPowerUpId)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::PowerUpHub, InPowerUpId);
}

void UPlayerBallStateMove::OnTourniquet(float TourniquetValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Tourniquet);
}

