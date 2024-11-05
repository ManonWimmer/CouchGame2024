// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateSnapping.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "PlayerBall/Datas/PlayerBallData.h"


// Sets default values for this component's properties
UPlayerBallStateSnapping::UPlayerBallStateSnapping()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, TEXT("PlayerState : Snapping"));
	
	if (Pawn != nullptr)
	{
		Pawn->CanGrappling = true;
		Pawn->CanBeGrappled = true;

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.AddDynamic(this, &UPlayerBallStateSnapping::OnEndSnapping);
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateSnapping::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateSnapping::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.AddDynamic(this, &UPlayerBallStateSnapping::OnBumped);
		}
		
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateSnapping::OnPunch);
		Pawn->OnGrapplingActionStarted.AddDynamic(this, &UPlayerBallStateSnapping::OnGrappling);
		Pawn->OnGrappledActionStarted.AddDynamic(this, &UPlayerBallStateSnapping::OnGrappled);
	}

	if (Pawn->BehaviorElementReactions != nullptr)
	{
		if (Pawn->BehaviorElementReactions->SnappingPlayerBall == nullptr)
		{
			StateMachine->ChangeState(EPlayerBallStateID::Idle);
		}
	}
}

void UPlayerBallStateSnapping::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnReceiveSnappingAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnEndSnapping);
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnImpacted);
			Pawn->BehaviorElementReactions->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnBumped);
		}
		
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnPunch);
		Pawn->OnGrapplingActionStarted.RemoveDynamic(this, &UPlayerBallStateSnapping::OnGrappling);
		Pawn->OnGrappledActionStarted.RemoveDynamic(this, &UPlayerBallStateSnapping::OnGrappled);

		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->SnappingPlayerBall = nullptr;
		}
	}
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

	Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -(Pawn->BehaviorMovements->AngularRollForce / Pawn->GetPlayerBallData()->SnapControlMoveRollDivider), NAME_None, true);
}

void UPlayerBallStateSnapping::SnappingEffect(float DeltaTime)
{
	if (Pawn->BehaviorElementReactions == nullptr)	return;
	
	if (Pawn->BehaviorElementReactions->SnappingPlayerBall == nullptr)	return;
	
	FVector Start = Pawn->GetActorLocation();
	FVector End = Pawn->BehaviorElementReactions->SnappingPlayerBall->GetActorLocation();

	FVector Dir = End - Start;

	float TempX = Dir.Y;

	Dir.Y = Dir.X;
	Dir.X = -TempX;

	Dir.Z = 0.f;

	Dir.Normalize();

	//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5.f);
	
	Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * Pawn->GetPlayerBallData()->SnapAngularForce, NAME_None, true);
}

void UPlayerBallStateSnapping::OnEndSnapping(float InSnappingValue)
{
	if (StateMachine == nullptr)	return;
	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);
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
