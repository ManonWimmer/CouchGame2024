// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateGrappling.h"

#include "CableComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"


// Sets default values for this component's properties
UPlayerBallStateGrappling::UPlayerBallStateGrappling()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

EPlayerBallStateID UPlayerBallStateGrappling::GetStateID() const
{
	return EPlayerBallStateID::Grappling;
}

void UPlayerBallStateGrappling::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

void UPlayerBallStateGrappling::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("PlayerState : Grappling"));

	if (Pawn != nullptr && Pawn->GrappledPlayerBall != nullptr)
	{
		Pawn->OnGrapplingAction.AddDynamic(this, &UPlayerBallStateGrappling::OnEndGrappling);
		Pawn->OnStunnedAction.AddDynamic(this, &UPlayerBallStateGrappling::OnStunned);
		Pawn->OnImpactAction.AddDynamic(this, &UPlayerBallStateGrappling::OnImpacted);

		Pawn->GrapplingCable->SetHiddenInGame(false);
		SetCable();

		FVector Direction = Pawn->GetActorLocation() - Pawn->GrappledPlayerBall->GetActorLocation();
		Pawn->CurrentGrapplingAngle = atan2f(Direction.Y, Direction.X);
		Pawn->CurrentGrapplingAngularVelocity = 0.f;

		Pawn->GrappledPlayerBall->ReceiveGrappledAction(1.f);
	}
}

void UPlayerBallStateGrappling::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("c ciao"));

	if (Pawn != nullptr)
	{
		Pawn->OnGrapplingAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnEndGrappling);
		Pawn->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnStunned);
		Pawn->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnImpacted);

		Pawn->IsGrappling = false;

		Pawn->LastAngle = Pawn->CurrentGrapplingAngle;
		SetGrapplingVelocityAndAngle(GetWorld()->DeltaTimeSeconds);

		if (Pawn->CurrentGrapplingAngle > Pawn->LastAngle)
			Pawn->AngleRotate = 90.f;
		else
			Pawn->AngleRotate = -90.f;

		Pawn->GrapplingCable->SetHiddenInGame(true);

		Pawn->ReleaseDirection = Pawn->GetActorLocation() + (Pawn->GetActorLocation() - Pawn->GrappledPlayerBall->
				GetActorLocation()).
			RotateAngleAxis(Pawn->AngleRotate, FVector(1, 0, 0));
		Pawn->ReleaseDirection.Z = 0.f;

		Pawn->SphereCollision->AddImpulse(
			Pawn->ReleaseDirection.GetSafeNormal(0.0001f) * FMath::Abs(Pawn->CurrentGrapplingAngularVelocity) * Pawn->
			GrapplingReleaseForce,
			"None", true);

		Pawn->CurrentGrapplingAngle = 0.f;
		Pawn->CurrentGrapplingAngularVelocity = 0.f;

		Pawn->GrappledPlayerBall->ReceiveGrappledAction(0.f);

		// Reset Grappled Player & Grappling Player
		Pawn->GrappledPlayerBall->GrapplingPlayerBall = nullptr;
		Pawn->GrappledPlayerBall = nullptr;
	}
}

void UPlayerBallStateGrappling::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Pawn->GrappledPlayerBall == nullptr)	return;
	
	SetGrapplingVelocityAndAngle(GetWorld()->DeltaTimeSeconds);

	if (TempGrapplingAngle == 0.f) return;

	Pawn->GrapplingOffset = FVector(FMath::Cos(TempGrapplingAngle), FMath::Sin(TempGrapplingAngle), 0)
		* Pawn->CableLength;

	float DistanceBetweenGrappledPlayers = FVector::Dist(Pawn->GetActorLocation(),
	                                                     Pawn->GrappledPlayerBall->GetActorLocation());

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, Pawn->GrapplingOffset.ToString());

	FVector NormalizedDirection = (Pawn->GetActorLocation() - Pawn->GrappledPlayerBall->GetActorLocation()).
		GetSafeNormal();

	// More or less cord
	if (FMath::Abs(Pawn->MoreLessValue) > 0.5f)
	{
		if (Pawn->MoreLessValue > 0.f && DistanceBetweenGrappledPlayers > Pawn->MinCableDistance) // Less cord
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Less cord");

			Pawn->GrapplingOffset -= NormalizedDirection * Pawn->MoreOrLessCablePerFrame;
		}
		else if (Pawn->MoreLessValue < 0.f && DistanceBetweenGrappledPlayers < Pawn->MaxCableDistance) // More cord
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "More cord");

			Pawn->GrapplingOffset += NormalizedDirection * Pawn->MoreOrLessCablePerFrame;
		}
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);

	FVector SphereCenter = Pawn->GrappledPlayerBall->GetActorLocation() + Pawn->GrapplingOffset;
	float SphereRadius = Pawn->SphereCollision->GetScaledSphereRadius() + 0.2f;

	FCollisionObjectQueryParams ObjectQueryParams(ECC_GameTraceChannel2);    // Look only for walls

	// Detect Collision With sphere overlap
	bool bHasDetected = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		SphereCenter,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(SphereRadius),
		CollisionParams
	);

	// Dessine une sphère de débogage
	DrawDebugSphere(
		GetWorld(),
		SphereCenter,
		SphereRadius,
		24,                        // Nombre de segments pour la sphère (plus élevé = sphère plus détaillée)
		bHasDetected ? FColor::Red : FColor::Green,  // Rouge si collision détectée, sinon vert
		false,                     // Persistent lines (false pour qu'elle disparaisse après un temps)
		1.0f,                      // Durée en secondes de la sphère de débogage
		0,                         // Prio
		1.0f                       // Épaisseur de la ligne
	);

	if(bHasDetected)
	{
		Pawn->SphereCollision->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
		Pawn->CurrentGrapplingAngularVelocity = 0.f;
		return;
	}

	//TObjectPtr<FHitResult> OutSweepHitResult;
	Pawn->SetActorLocation(Pawn->GrappledPlayerBall->GetActorLocation() + Pawn->GrapplingOffset);
	// Test de MoveComponent et AddForce -> ça fait nimp lol
	//Pawn->SetActorLocation(Pawn->GrappledPlayerBall->GetActorLocation() + Pawn->GrapplingOffset, true, OutSweepHitResult ); détecte bien les murs mais aussi les bumpers + un peu crade 
	Pawn->GrapplingCable->SetWorldLocation(Pawn->GetActorLocation());
	SetCable();

	Pawn->CurrentGrapplingAngularVelocity = TempGrapplingAngularVelocity;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Angle %f"), TempGrapplingAngle));
	Pawn->CurrentGrapplingAngle = TempGrapplingAngle;
	

	LastLocation = Pawn->GetActorLocation();
}

void UPlayerBallStateGrappling::OnEndGrappling(float InGrapplingValue) // Stop press input grappling -> Idle
{
	if (StateMachine == nullptr) return;

	if (InGrapplingValue == 0.0f)
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
}

void UPlayerBallStateGrappling::OnStunned(float StunnedValue) // hit by punch -> stunned
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateGrappling::OnImpacted(float ImpactedValue) // impact ball -> impacted
{
	if (StateMachine == nullptr) return;

	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateGrappling::SetCable()
{
	Pawn->GrapplingCable->SetWorldLocation(Pawn->GetActorLocation());

	if (Pawn->GrappledPlayerBall != nullptr)
	{
		Pawn->HookPoint = Pawn->GrappledPlayerBall->GetActorLocation();

		Pawn->CableLength = FVector::Dist(Pawn->GetActorLocation(), Pawn->HookPoint);

		//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta, FString::Printf(TEXT("Cable Length: %f"), Pawn->CableLength));
		Pawn->GrapplingCable->CableLength = Pawn->CableLength;

	
		Pawn->GrapplingCable->SetAttachEndToComponent(Pawn->GrappledPlayerBall->SphereMesh);
	}
}

void UPlayerBallStateGrappling::SetGrapplingVelocityAndAngle(float DeltaTime)
{
	TempGrapplingAngularVelocity = Pawn->GrapplingDamping * Pawn->CurrentGrapplingAngularVelocity + (Pawn->
			MoveXValue * -1 *
			Pawn->GrapplingForce) +
		Pawn->StartGrapplingForceFactorWhenAlreadyMoving * Pawn->GetVelocity().X;

	TempGrapplingAngle = Pawn->CurrentGrapplingAngle +  TempGrapplingAngularVelocity * DeltaTime;
}
