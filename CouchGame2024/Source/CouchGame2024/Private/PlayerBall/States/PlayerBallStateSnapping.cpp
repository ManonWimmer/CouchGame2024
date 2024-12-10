// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateSnapping.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PhysicsEngine/BodySetup.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"
#include "PlayerBall/Datas/PlayerBallData.h"


// Sets default values for this component's properties
UPlayerBallStateSnapping::UPlayerBallStateSnapping()
{
	
}

EPlayerBallStateID UPlayerBallStateSnapping::GetStateID() const
{
	return EPlayerBallStateID::Snapping;
}

void UPlayerBallStateSnapping::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateSnapping::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("PlayerState : Snapping"));

	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Snapping") );
	
	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.AddDynamic(this, &UPlayerBallStateSnapping::OnDeath);

		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = true;
			Pawn->BehaviorGrapple->CanBeGrappled = true;

			Pawn->BehaviorGrapple->OnGrapplingActionStarted.AddDynamic(this, &UPlayerBallStateSnapping::OnGrappling);
			Pawn->BehaviorGrapple->OnGrappledActionStarted.AddDynamic(this, &UPlayerBallStateSnapping::OnGrappled);
		}
		
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateSnapping::OnPunch);


		if (Pawn->BehaviorPowerUp != nullptr)
		{
			Pawn->BehaviorPowerUp->OnUsePowerUpAction.AddDynamic(this, &UPlayerBallStateSnapping::OnUsePowerUp);
		}
		
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.AddDynamic(this, &UPlayerBallStateSnapping::OnEndSnapping);
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateSnapping::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateSnapping::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.AddDynamic(this, &UPlayerBallStateSnapping::OnBumped);

			Pawn->BehaviorElementReactions->OnRailReaction.AddDynamic(this, &UPlayerBallStateSnapping::OnRail);

			Pawn->BehaviorElementReactions->OnTourniquetReaction.AddDynamic(this, &UPlayerBallStateSnapping::OnTourniquet);
			
			if (Pawn->BehaviorElementReactions->SnappingPlayerBall == nullptr)
			{
				StateMachine->ChangeState(EPlayerBallStateID::Idle);
			}
		}
	}
}

void UPlayerBallStateSnapping::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnDeathReaction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnDeath);
		
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnPunch);

		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->OnGrapplingActionStarted.RemoveDynamic(this, &UPlayerBallStateSnapping::OnGrappling);
			Pawn->BehaviorGrapple->OnGrappledActionStarted.RemoveDynamic(this, &UPlayerBallStateSnapping::OnGrappled);
		}

		if (Pawn->BehaviorPowerUp != nullptr)
		{
			Pawn->BehaviorPowerUp->OnUsePowerUpAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnUsePowerUp);
		}

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnEndSnapping);
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnBumped);

			Pawn->BehaviorElementReactions->OnRailReaction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnRail);

			Pawn->BehaviorElementReactions->OnTourniquetReaction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnTourniquet);


			Pawn->BehaviorElementReactions->SnappingPlayerBall = nullptr;
		}
	}

	SetRightMoveMaterial(false);
}

void UPlayerBallStateSnapping::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Move(DeltaTime);

	
	if (Pawn->GetVelocity().Length() >= Pawn->GetPlayerBallData()->MinVelocityToSnap)
	{
		SnappingEffect(DeltaTime);
	}

}

void UPlayerBallStateSnapping::Move(float DeltaTime)
{
	if (Pawn == nullptr)
		return;

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

	Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -(Pawn->BehaviorMovements->GetContextRollForce() / Pawn->GetPlayerBallData()->SnapControlMoveRollDivider), NAME_None, true);
}

void UPlayerBallStateSnapping::SnappingEffect(float DeltaTime)
{
	if (Pawn->BehaviorElementReactions == nullptr)	return;
	
	if (Pawn->BehaviorElementReactions->SnappingPlayerBall == nullptr)	return;

	if (Pawn->IsLocked())	return;
	
	FVector Start = Pawn->GetActorLocation();
	FVector End = Pawn->BehaviorElementReactions->SnappingPlayerBall->GetActorLocation();

	FVector Dir = End - Start;

	float TempX = Dir.Y;

	Dir.Y = Dir.X;
	Dir.X = -TempX;

	Dir.Z = 0.f;

	Dir.Normalize();

	//DrawDebugLine(Pawn->GetWorld(), Start, End, FColor::Blue, false, 5.f);
	
	Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * Pawn->GetPlayerBallData()->SnapAngularForce, NAME_None, true);
}

void UPlayerBallStateSnapping::OnEndSnapping(float InSnappingValue)
{
	if (StateMachine == nullptr)	return;
	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void UPlayerBallStateSnapping::SetRightMoveMaterial(bool IsBrakeDirection)
{
	if (Pawn == nullptr)	return;
	if (Pawn->SphereCollision == nullptr)	return;
	if (Pawn->GetPlayerBallData() == nullptr)	return;
	if (Pawn->GetPlayerBallData()->BrakeDirectionPhysicsAsset == nullptr)	return;
	if (Pawn->GetPlayerBallData()->ClassicPhysicsAsset == nullptr)	return;
	
	if (IsBrakeDirection)
	{
		if (Pawn->SphereCollision->GetBodySetup()->GetPhysMaterial() != Pawn->GetPlayerBallData()->BrakeDirectionPhysicsAsset)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Change material to Brake direction"));
			Pawn->SphereCollision->SetPhysMaterialOverride(Pawn->GetPlayerBallData()->BrakeDirectionPhysicsAsset);
		}
	}
	else
	{
		if (Pawn->SphereCollision->GetBodySetup()->GetPhysMaterial() != Pawn->GetPlayerBallData()->ClassicPhysicsAsset)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("Change material to classic direction"));

			Pawn->SphereCollision->SetPhysMaterialOverride(Pawn->GetPlayerBallData()->ClassicPhysicsAsset);
		}
	}
}

void UPlayerBallStateSnapping::OnStunned(float StunnedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateSnapping::OnPunch(float PunchValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Punch);
}

void UPlayerBallStateSnapping::OnImpacted(float ImpactedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateSnapping::OnBumped(float BumpedValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Bumped);
}

void UPlayerBallStateSnapping::OnGrappling(float InGrapplingValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappling);
}

void UPlayerBallStateSnapping::OnGrappled(float InGrappledValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappled);
}

void UPlayerBallStateSnapping::OnDeath(float DeathValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Death);
}

void UPlayerBallStateSnapping::OnRail(float RailDirectionValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Rail, RailDirectionValue);
}

void UPlayerBallStateSnapping::OnUsePowerUp(float InPowerUpId)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::PowerUpHub, InPowerUpId);
}

void UPlayerBallStateSnapping::OnTourniquet(float TourniquetValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Tourniquet);
}
