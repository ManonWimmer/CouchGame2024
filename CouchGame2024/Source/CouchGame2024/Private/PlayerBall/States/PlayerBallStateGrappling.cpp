// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateGrappling.h"

#include "CableComponent.h"
#include "Components/SphereComponent.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"


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

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateGrappling::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateGrappling::OnImpacted);
		}
		
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;

			Pawn->BehaviorGrapple->OnGrapplingActionEnded.AddDynamic(this, &UPlayerBallStateGrappling::OnEndGrappling);

			
			if (Pawn->BehaviorGrapple->GrappledPlayerBall != nullptr)
			{
				Pawn->GrapplingCable->SetHiddenInGame(false);
				SetCable();

				FVector Direction = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation();
				Pawn->BehaviorGrapple->CurrentGrapplingAngle = atan2f(Direction.Y, Direction.X);
				Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = 0.f;

				if (Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple != nullptr)
				{
					Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple->ReceiveGrappledActionStarted(1.f);
				}
			}
		}

	}
}

void UPlayerBallStateGrappling::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("c ciao"));

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnImpacted);
		}


		
		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->OnGrapplingActionEnded.RemoveDynamic(this, &UPlayerBallStateGrappling::OnEndGrappling);
			
			Pawn->BehaviorGrapple->IsGrappling = false;
			Pawn->BehaviorGrapple->LastAngle = Pawn->BehaviorGrapple->CurrentGrapplingAngle;

			SetGrapplingVelocityAndAngle(GetWorld()->DeltaTimeSeconds);

			if (Pawn->BehaviorGrapple->CurrentGrapplingAngle > Pawn->BehaviorGrapple->LastAngle)
				Pawn->BehaviorGrapple->AngleRotate = 90.f;
			else
				Pawn->BehaviorGrapple->AngleRotate = -90.f;

			
			Pawn->GrapplingCable->SetHiddenInGame(true);

			/*
			Pawn->ReleaseDirection = Pawn->GetActorLocation() + (Pawn->GetActorLocation() - Pawn->GrappledPlayerBall->
					GetActorLocation()).
				RotateAngleAxis(Pawn->AngleRotate, FVector(1, 0, 0));
				*/
			// ------ Changed Part ----------
			Pawn->BehaviorGrapple->ReleaseDirection = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->GrappledPlayerBall->
					GetActorLocation();
			float TempX = Pawn->BehaviorGrapple->ReleaseDirection.X;
			Pawn->BehaviorGrapple->ReleaseDirection.X = -Pawn->BehaviorGrapple->ReleaseDirection.Y;	// To get perpendicular vector of n : n(x, y) -> np(-y, x)
			Pawn->BehaviorGrapple->ReleaseDirection.Y = TempX;
			Pawn->BehaviorGrapple->ReleaseDirection.Z = 0.f;

			Pawn->BehaviorGrapple->ReleaseDirection *= FMath::Sign(Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity);
			DrawDebugLine(GetWorld(), Pawn->GetActorLocation(), Pawn->GetActorLocation() + Pawn->BehaviorGrapple->ReleaseDirection * 500.f, FColor::Red, false, 5.f);
			// Changed Part
			
			Pawn->SphereCollision->AddImpulse(
				Pawn->BehaviorGrapple->ReleaseDirection.GetSafeNormal(0.0001f) * FMath::Abs(Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity) *
				Pawn->BehaviorGrapple->GrapplingReleaseForce,
				"None", true);

			Pawn->BehaviorGrapple->CurrentGrapplingAngle = 0.f;
			Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = 0.f;

			if (Pawn->BehaviorGrapple->GrappledPlayerBall != nullptr)
			{
				if (Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple != nullptr)
				{
					Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple->ReceiveGrappledActionEnded(0.f);
				}

				// Reset Grappled Player & Grappling Player
				//Pawn->GrappledPlayerBall->GrapplingPlayerBall = nullptr;

				UE_LOG(LogTemp, Warning, TEXT("UnSet grappledPlayerBall") );
				Pawn->BehaviorGrapple->GrappledPlayerBall = nullptr;
			}
		}
	}
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "End Grappling");
}

void UPlayerBallStateGrappling::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if (Pawn->BehaviorGrapple == nullptr)	return;
	
	if (Pawn->BehaviorGrapple->GrappledPlayerBall == nullptr)	return;
	
	SetGrapplingVelocityAndAngle(GetWorld()->DeltaTimeSeconds);

	if (TempGrapplingAngle == 0.f) return;

	Pawn->BehaviorGrapple->GrapplingOffset = FVector(FMath::Cos(TempGrapplingAngle), FMath::Sin(TempGrapplingAngle), 0)
		* Pawn->BehaviorGrapple->CableLength;

	float DistanceBetweenGrappledPlayers = FVector::Dist(Pawn->GetActorLocation(),
	                                                     Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation());

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, Pawn->GrapplingOffset.ToString());

	FVector NormalizedDirection = (Pawn->GetActorLocation() - Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation()).
		GetSafeNormal();

	// More or less cord
	if (FMath::Abs(Pawn->BehaviorGrapple->MoreLessValue) > 0.5f)
	{
		if (Pawn->BehaviorGrapple->MoreLessValue > 0.f && DistanceBetweenGrappledPlayers > Pawn->BehaviorGrapple->MinCableDistance) // Less cord
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Less cord");

			Pawn->BehaviorGrapple->GrapplingOffset -= NormalizedDirection * Pawn->BehaviorGrapple->MoreOrLessCablePerFrame;
		}
		else if (Pawn->BehaviorGrapple->MoreLessValue < 0.f && DistanceBetweenGrappledPlayers < Pawn->BehaviorGrapple->MaxCableDistance) // More cord
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "More cord");

			Pawn->BehaviorGrapple->GrapplingOffset += NormalizedDirection * Pawn->BehaviorGrapple->MoreOrLessCablePerFrame;
		}
	}

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);

	FVector SphereCenter = Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation() + Pawn->BehaviorGrapple->GrapplingOffset;
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

	/*
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
	*/
	
	if(bHasDetected)
	{
		Pawn->SphereCollision->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
		Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = 0.f;
		return;
	}

	//TObjectPtr<FHitResult> OutSweepHitResult;
	Pawn->SetActorLocation(Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation() + Pawn->BehaviorGrapple->GrapplingOffset);
	// Test de MoveComponent et AddForce -> ça fait nimp lol
	//Pawn->SetActorLocation(Pawn->GrappledPlayerBall->GetActorLocation() + Pawn->GrapplingOffset, true, OutSweepHitResult ); détecte bien les murs mais aussi les bumpers + un peu crade 
	Pawn->GrapplingCable->SetWorldLocation(Pawn->GetActorLocation());
	SetCable();

	Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = TempGrapplingAngularVelocity;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Angle %f"), TempGrapplingAngle));
	Pawn->BehaviorGrapple->CurrentGrapplingAngle = TempGrapplingAngle;
	

	LastLocation = Pawn->GetActorLocation();
}

void UPlayerBallStateGrappling::OnEndGrappling(float InGrapplingValue) // Stop press input grappling -> Idle
{
	if (StateMachine == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Stop by release input or by grappled cause in grappling") );
	
	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void UPlayerBallStateGrappling::OnStunned(float StunnedValue) // hit by punch -> stunned
{
	if (StateMachine == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Stop by stunned in grappling") );
	
	StateMachine->ChangeState(EPlayerBallStateID::Stun, StunnedValue);
}

void UPlayerBallStateGrappling::OnImpacted(float ImpactedValue) // impact ball -> impacted
{
	if (StateMachine == nullptr) return;

	//UE_LOG(LogTemp, Warning, TEXT("Stop by impact in grappling") );
	
	StateMachine->ChangeState(EPlayerBallStateID::Impact);
}

void UPlayerBallStateGrappling::SetCable()
{
	Pawn->GrapplingCable->SetWorldLocation(Pawn->GetActorLocation());

	if (Pawn->BehaviorGrapple == nullptr) return;
	
	if (Pawn->BehaviorGrapple->GrappledPlayerBall != nullptr)
	{
		Pawn->BehaviorGrapple->HookPoint = Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation();

		Pawn->BehaviorGrapple->CableLength = FVector::Dist(Pawn->GetActorLocation(), Pawn->BehaviorGrapple->HookPoint);

		//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta, FString::Printf(TEXT("Cable Length: %f"), Pawn->CableLength));
		Pawn->GrapplingCable->CableLength = Pawn->BehaviorGrapple->CableLength;

	
		Pawn->GrapplingCable->SetAttachEndToComponent(Pawn->BehaviorGrapple->GrappledPlayerBall->SphereMesh);
	}
}

void UPlayerBallStateGrappling::SetGrapplingVelocityAndAngle(float DeltaTime)
{
	if (Pawn->BehaviorMovements == nullptr)	return;

	if (Pawn->BehaviorGrapple == nullptr)	return;
	
	TempGrapplingAngularVelocity = Pawn->BehaviorGrapple->GrapplingDamping * Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity + (Pawn->BehaviorMovements->MoveXValue * -1 *
			Pawn->BehaviorGrapple->GrapplingForce) +
		Pawn->BehaviorGrapple->StartGrapplingForceFactorWhenAlreadyMoving * Pawn->GetVelocity().X;

	TempGrapplingAngle = Pawn->BehaviorGrapple->CurrentGrapplingAngle +  TempGrapplingAngularVelocity * DeltaTime;
}
