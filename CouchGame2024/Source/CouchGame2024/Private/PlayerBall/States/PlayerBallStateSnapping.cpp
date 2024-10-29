// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateSnapping.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


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
		Pawn->OnReceiveSnappingAction.AddDynamic(this, &UPlayerBallStateSnapping::OnEndSnapping);
		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateSnapping::OnStunned);
		Pawn->OnPunchAction.AddDynamic(this, &UPlayerBallStateSnapping::OnPunch);
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateSnapping::OnImpacted);
		Pawn->OnBumperReaction.AddDynamic(this, &UPlayerBallStateSnapping::OnBumped);
		Pawn->OnGrapplingAction.AddDynamic(this, &UPlayerBallStateSnapping::OnGrappling);
		Pawn->OnGrappledAction.AddDynamic(this, &UPlayerBallStateSnapping::OnGrappled);
	}

	if (Pawn->SnappingPlayerBall == nullptr)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateSnapping::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		Pawn->OnReceiveSnappingAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnEndSnapping);
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnStunned);
		Pawn->OnPunchAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnPunch);
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnImpacted);
		Pawn->OnBumperReaction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnBumped);
		Pawn->OnGrapplingAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnGrappling);
		Pawn->OnGrappledAction.RemoveDynamic(this, &UPlayerBallStateSnapping::OnGrappled);

		Pawn->SnappingPlayerBall = nullptr;
	}
}

void UPlayerBallStateSnapping::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	Move(DeltaTime);

	if (Pawn->GetVelocity().Length() >= Pawn->MinVelocityToSnap)
	{
		SnappingEffect(DeltaTime);
	}

}

void UPlayerBallStateSnapping::Move(float DeltaTime)
{
	if (Pawn->PawnMovement == nullptr)
		return;

	FVector FwdVect(1.f, 0.f, 0.f);

	FVector RightVect(0.f, -1.f, 0.f);
	
	FVector Dir = (FwdVect * Pawn->MoveXValue) + (RightVect * Pawn->MoveYValue);	// Get ball roll dir
	
	if (Pawn->SphereCollision == nullptr)
		return;

	bool SameDirectionX = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X <= 0 && Dir.X >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().X >= 0 && Dir.X <= 0);
	bool SameDirectionY = (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().Y <= 0 && Dir.Y >= 0) || (Pawn->SphereCollision->GetPhysicsAngularVelocityInDegrees().Y >= 0 && Dir.Y <= 0);

	if (!SameDirectionX)	// May Increase roll X if oppositeDirection
	{
		Dir.X *= Pawn->BraqueDirectionForceMultiplier;
	}
	if (!SameDirectionY)	// May Increase roll Y if oppositeDirection
	{
		Dir.Y *= Pawn->BraqueDirectionForceMultiplier;
	}

	Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * -(Pawn->AngularRollForce / Pawn->SnapControlMoveRollDivider), NAME_None, true);
}

void UPlayerBallStateSnapping::SnappingEffect(float DeltaTime)
{
	if (Pawn->SnappingPlayerBall == nullptr)	return;
	
	FVector Start = Pawn->GetActorLocation();
	FVector End = Pawn->SnappingPlayerBall->GetActorLocation();

	FVector Dir = End - Start;

	float TempX = Dir.Y;

	Dir.Y = Dir.X;
	Dir.X = -TempX;

	Dir.Z = 0.f;

	Dir.Normalize();

	//DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 5.f);
	
	Pawn->SphereCollision->AddAngularImpulseInDegrees(Dir * DeltaTime * Pawn->SnapAngularForce, NAME_None, true);
}

void UPlayerBallStateSnapping::OnEndSnapping(float InSnappingValue)
{
	if (StateMachine == nullptr)	return;

	if (InSnappingValue > 0.f)	return;
	
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

	if (InGrapplingValue == 1.f)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Grappling);
	}
}

void UPlayerBallStateSnapping::OnGrappled(float InGrappledValue)
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Grappled);
}
