// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/States/PlayerBallStateGrappling.h"

#include "CableComponent.h"
#include "Components/SphereComponent.h"
#include "GrapplingHook/Hookable.h"
#include "PinballElements/Elements/PillarElement.h"
#include "PlayerBall/PlayerBall.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"

#include "Score/GlobalScoreSubsystem.h"


// Sets default values for this component's properties
UPlayerBallStateGrappling::UPlayerBallStateGrappling()
{
}


EPlayerBallStateID UPlayerBallStateGrappling::GetStateID() const
{
	return EPlayerBallStateID::Grappling;
}

void UPlayerBallStateGrappling::StateInit(UPlayerBallStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		ScoreSubsystem = World->GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	}
}

void UPlayerBallStateGrappling::StateEnter(EPlayerBallStateID PreviousState)
{
	Super::StateEnter(PreviousState);

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("PlayerState : Grappling"));
	UE_LOG(LogTemp, Warning, TEXT("Enter PlayerState : Grappling"));

	if (Pawn != nullptr)
	{
		Pawn->PlayGrapplingGrabGamefeelEffectsBlueprint();
		
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.AddDynamic(this, &UPlayerBallStateGrappling::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.AddDynamic(this, &UPlayerBallStateGrappling::OnImpacted);
		}

		if (Pawn->BehaviorGrapple != nullptr)
		{
			// Reset bools
			Pawn->BehaviorGrapple->CanGrappling = false;
			Pawn->BehaviorGrapple->CanBeGrappled = false;

			Pawn->BehaviorGrapple->OnGrapplingActionEnded.AddDynamic(this, &UPlayerBallStateGrappling::OnEndGrappling);

			// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
			/*
			if (Pawn->BehaviorGrapple->GrappledPlayerBall != nullptr)
			{
				Pawn->GrapplingCable->SetHiddenInGame(false);
				SetCable();

				FVector Direction = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->GrappledPlayerBall->
				                                                     GetActorLocation();
				Pawn->BehaviorGrapple->CurrentGrapplingAngle = atan2f(Direction.Y, Direction.X);
				Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = 0.f;

				if (Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple != nullptr)
				{
					Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple->ReceiveGrappledActionStarted(1.f);
				}
			}
			*/
			// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

			// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
			if (Pawn->BehaviorGrapple->HookInterface != nullptr)
			{
				// Hide cable
				Pawn->GrapplingCable->SetHiddenInGame(false);
				SetCable();

				// Start values 
				FVector Direction = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->HookInterface->GetHookPosition();
				Pawn->BehaviorGrapple->CurrentGrapplingAngle = atan2f(Direction.Y, Direction.X);
				Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = 0.f;

				StartAngle = Pawn->BehaviorGrapple->CurrentGrapplingAngle;

				if (Pawn->BehaviorGrapple->HookInterface->IsPillar())
				{
					APillarElement* Pillar = Cast<APillarElement>(Pawn->BehaviorGrapple->HookObject);
					if (Pillar)
					{
						if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "set state machine");
						Pillar->PlayerStateMachineOnPillar = StateMachine;
					}
				}
			}
			// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
		}


		FVector LastPosToCenter = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->HookInterface->GetHookPosition();
		float LastPosAngle = FMath::Atan2(LastPosToCenter.Y, LastPosToCenter.X);
		FVector LastPosToCenterNorm = LastPosToCenter.GetSafeNormal();
		FVector NormalizedLastLocation = LastLocation.GetSafeNormal();
		FVector NormalizeVelocity = Pawn->GetVelocity().GetSafeNormal();
		
		// (U.X * V.Y) - (U.Y * V.X)
		float truc = (LastPosToCenterNorm.X * NormalizeVelocity.Y) - (LastPosToCenterNorm.Y * NormalizeVelocity.X);
		
		RotationDirection = truc > 0 ? 1.0f : -1.0f;
	}
}

void UPlayerBallStateGrappling::StateExit(EPlayerBallStateID NextState)
{
	Super::StateExit(NextState);

	if (Pawn != nullptr)
	{
		if (Pawn->BehaviorElementReactions != nullptr)
		{
			Pawn->BehaviorElementReactions->OnStunnedAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnStunned);
			Pawn->BehaviorElementReactions->OnImpactAction.RemoveDynamic(this, &UPlayerBallStateGrappling::OnImpacted);
		}

		if (Pawn->BehaviorGrapple != nullptr)
		{
			Pawn->BehaviorGrapple->OnGrapplingActionEnded.RemoveDynamic(
				this, &UPlayerBallStateGrappling::OnEndGrappling);

			Pawn->BehaviorGrapple->IsGrappling = false;
			Pawn->BehaviorGrapple->LastAngle = Pawn->BehaviorGrapple->CurrentGrapplingAngle;

			// Get new temp angle & velocity
			SetGrapplingVelocityAndAnglePillar(Pawn->GetWorld()->DeltaTimeSeconds);

			// Get angle rotate left or right depending on last & current angle
			if (Pawn->BehaviorGrapple->CurrentGrapplingAngle > Pawn->BehaviorGrapple->LastAngle)
				Pawn->BehaviorGrapple->AngleRotate = 90.f;
			else
				Pawn->BehaviorGrapple->AngleRotate = -90.f;

			// Hide cable
			Pawn->GrapplingCable->SetHiddenInGame(true);

			// Get release direction
			// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
			// Pawn->BehaviorGrapple->ReleaseDirection = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation();
			// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

			// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
			Pawn->BehaviorGrapple->ReleaseDirection = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->HookInterface->
				GetHookPosition();
			// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

			float TempX = Pawn->BehaviorGrapple->ReleaseDirection.X;
			Pawn->BehaviorGrapple->ReleaseDirection.X = -Pawn->BehaviorGrapple->ReleaseDirection.Y;
			// To get perpendicular vector of n : n(x, y) -> np(-y, x)
			Pawn->BehaviorGrapple->ReleaseDirection.Y = TempX;
			Pawn->BehaviorGrapple->ReleaseDirection.Z = 0.f;

			Pawn->BehaviorGrapple->ReleaseDirection *= FMath::Sign(
				Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity);
			DrawDebugLine(Pawn->GetWorld(), Pawn->GetActorLocation(),
			              Pawn->GetActorLocation() + Pawn->BehaviorGrapple->ReleaseDirection * 500.f, FColor::Red,
			              false, 5.f);

			float ReleaseForce = 0.f;
			
			// Add impulse in relase direction
			if (Pawn->BehaviorGrapple->IsHookingPillar)
			{
				ReleaseForce = Pawn->BehaviorGrapple->GrapplingReleaseForcePillar;
			}
			else
			{
				ReleaseForce = Pawn->BehaviorGrapple->GrapplingReleaseForceNotPillar;
			}
			
			Pawn->SphereCollision->AddImpulse(
				Pawn->BehaviorGrapple->ReleaseDirection.GetSafeNormal(0.0001f) * FMath::Abs(
					Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity) *
				ReleaseForce,
				"None", true);

			// Reset angle & velocity
			Pawn->BehaviorGrapple->CurrentGrapplingAngle = 0.f;
			Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = 0.f;

			// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
			// Reset grappled ball
			/*
			if (Pawn->BehaviorGrapple->GrappledPlayerBall != nullptr)
			{
				if (Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple != nullptr)
				{
					Pawn->BehaviorGrapple->GrappledPlayerBall->BehaviorGrapple->ReceiveGrappledActionEnded(0.f);
				}

				// Reset Grappled Player & Grappling Player
				Pawn->GrappledPlayerBall->GrapplingPlayerBall = nullptr;

				UE_LOG(LogTemp, Warning, TEXT("UnSet grappledPlayerBall"));
				Pawn->BehaviorGrapple->GrappledPlayerBall = nullptr;
			}
			*/
			// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

			// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
			if (Pawn->BehaviorGrapple->HookInterface->IsPillar())
			{
				APillarElement* Pillar = Cast<APillarElement>(Pawn->BehaviorGrapple->HookObject);
				if (Pillar)
				{
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, "reset state machine");
					Pillar->PlayerStateMachineOnPillar = nullptr;
				}
			}
			
			// Reset hook point
			if (Pawn->BehaviorGrapple->HookInterface != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("UnSet hook point interface"));
				Pawn->BehaviorGrapple->HookInterface = nullptr;
			}

			if (Pawn->BehaviorGrapple->HookObject != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("UnSet hook point object"));
				Pawn->BehaviorGrapple->HookObject = nullptr;
			}
			// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

			Pawn->BehaviorGrapple->StartGrapplingCooldown();
			CurrentTimeOnPillar = 0.f;
		}
	}

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "End Grappling");
}

void UPlayerBallStateGrappling::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (Pawn->BehaviorGrapple == nullptr) return;

	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
	//if (Pawn->BehaviorGrapple->GrappledPlayerBall == nullptr) return;
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	if (Pawn->BehaviorGrapple->HookInterface == nullptr) return;
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

	// Get temp angle, velocity & offset
	if (Pawn->BehaviorGrapple->IsHookingPillar)
	{
		CurrentTimeOnPillar += DeltaTime;
		SetGrapplingVelocityAndAnglePillar(DeltaTime);
	}
	else
	{
		SetGrapplingVelocityAndAngleNotPillar(DeltaTime);
	}

	if (TempGrapplingAngle == 0.f) return;

	Pawn->BehaviorGrapple->GrapplingOffset = FVector(FMath::Cos(TempGrapplingAngle), FMath::Sin(TempGrapplingAngle), 0)
		* Pawn->BehaviorGrapple->CableLength;

	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
	// float DistanceBetweenGrappledPlayers = FVector::Dist(Pawn->GetActorLocation(), Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation());
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	float DistanceBetweenPlayerAndHookPoint = FVector::Dist(Pawn->GetActorLocation(),
	                                                        Pawn->BehaviorGrapple->HookInterface->GetHookPosition());
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, Pawn->GrapplingOffset.ToString());

	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
	// FVector NormalizedDirection = (Pawn->GetActorLocation() - Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation()).GetSafeNormal();
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	FVector NormalizedDirection = (Pawn->GetActorLocation() - Pawn->BehaviorGrapple->HookInterface->GetHookPosition()).
		GetSafeNormal();
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

	if (Pawn->BehaviorGrapple->IsHookingPillar)
	{
		// More or less cord depending on input
		if (FMath::Abs(Pawn->BehaviorGrapple->MoreLessValue) > 0.5f)
		{
			if (Pawn->BehaviorGrapple->MoreLessValue > 0.f && DistanceBetweenPlayerAndHookPoint > Pawn->BehaviorGrapple
				->
				MinCableDistance) // Less cord
			{
				//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Less cord");

				Pawn->BehaviorGrapple->GrapplingOffset -= NormalizedDirection * Pawn->BehaviorGrapple->
					MoreOrLessCablePerFrame;
			}
			else if (Pawn->BehaviorGrapple->MoreLessValue < 0.f && DistanceBetweenPlayerAndHookPoint < Pawn->
				BehaviorGrapple->
				MaxCableDistance) // More cord
			{
				//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "More cord");

				Pawn->BehaviorGrapple->GrapplingOffset += NormalizedDirection * Pawn->BehaviorGrapple->
					MoreOrLessCablePerFrame;
			}
		}

		// Gain points on pillar
		GainPillarPoints();
	}

	// Stop movement if wall detected & return
	if (DetectWalls())
	{
		Pawn->SphereCollision->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
		Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = 0.f;
		OnEndGrappling(1.f);
		return;
	}

	// Update player location
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
	// Pawn->SetActorLocation(Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation() + Pawn->BehaviorGrapple->GrapplingOffset);
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	Pawn->SetActorLocation(
		Pawn->BehaviorGrapple->HookInterface->GetHookPosition() + Pawn->BehaviorGrapple->GrapplingOffset);

	if (!Pawn->BehaviorGrapple->IsHookingPillar && FMath::Abs(StartAngle - Pawn->BehaviorGrapple->CurrentGrapplingAngle)
		>
		FMath::DegreesToRadians(90.f))
	{
		StateMachine->ChangeState(EPlayerBallStateID::Idle);
	}
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

	// Update cable
	Pawn->GrapplingCable->SetWorldLocation(Pawn->GetActorLocation());
	SetCable();

	// Can set temp velocity & angle if didn't return before
	Pawn->BehaviorGrapple->CurrentGrapplingAngularVelocity = TempGrapplingAngularVelocity;
	Pawn->BehaviorGrapple->CurrentGrapplingAngle = TempGrapplingAngle;

	// Update last location
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

void UPlayerBallStateGrappling::SetCable() // Same for Pillar & Not Pillar
{
	// Set cable on player
	Pawn->GrapplingCable->SetWorldLocation(Pawn->GetActorLocation());

	if (Pawn->BehaviorGrapple == nullptr) return;

	// Set cable end location & length
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
	/*
	if (Pawn->BehaviorGrapple->GrappledPlayerBall != nullptr)
	{
		Pawn->BehaviorGrapple->HookPoint = Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation();

		Pawn->BehaviorGrapple->CableLength = FVector::Dist(Pawn->GetActorLocation(), Pawn->BehaviorGrapple->HookPoint);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta, FString::Printf(TEXT("Cable Length: %f"), Pawn->CableLength));
		Pawn->GrapplingCable->CableLength = Pawn->BehaviorGrapple->CableLength;

		Pawn->GrapplingCable->SetAttachEndToComponent(Pawn->BehaviorGrapple->GrappledPlayerBall->SphereMesh);
	}
	*/
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	if (Pawn->BehaviorGrapple->HookInterface != nullptr)
	{
		Pawn->BehaviorGrapple->HookPoint = Pawn->BehaviorGrapple->HookInterface->GetHookPosition();

		Pawn->BehaviorGrapple->CableLength = FVector::Dist(Pawn->GetActorLocation(), Pawn->BehaviorGrapple->HookPoint);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta, FString::Printf(TEXT("Cable Length: %f"), Pawn->CableLength));
		Pawn->GrapplingCable->CableLength = Pawn->BehaviorGrapple->CableLength;

		Pawn->GrapplingCable->SetAttachEndToComponent(
			Cast<AActor>(Pawn->BehaviorGrapple->HookObject)->GetRootComponent());
	}
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
}

void UPlayerBallStateGrappling::SetGrapplingVelocityAndAnglePillar(float DeltaTime)
{
	if (Pawn->BehaviorMovements == nullptr) return;
	if (Pawn->BehaviorGrapple == nullptr) return;

	// Get angular velocity with last velocity & input
	TempGrapplingAngularVelocity = Pawn->BehaviorGrapple->GrapplingDamping * Pawn->BehaviorGrapple->
		CurrentGrapplingAngularVelocity + (Pawn->BehaviorMovements->MoveXValue * -1 *
			Pawn->BehaviorGrapple->GrapplingPillarForce) +
		Pawn->BehaviorGrapple->StartGrapplingForceFactorWhenAlreadyMoving * Pawn->GetVelocity().X;
	// A modif pour prendre velocity y

	// Get new angle from new velocity
	TempGrapplingAngle = Pawn->BehaviorGrapple->CurrentGrapplingAngle + TempGrapplingAngularVelocity * DeltaTime;
}

void UPlayerBallStateGrappling::SetGrapplingVelocityAndAngleNotPillar(float DeltaTime)
{
	if (Pawn->BehaviorMovements == nullptr || Pawn->BehaviorGrapple == nullptr) return;

	//FVector LastPosToCenter = LastLocation - Pawn->BehaviorGrapple->HookInterface->GetHookPosition();
	//FVector LastPosToCenter = Pawn->GetActorLocation() - Pawn->BehaviorGrapple->HookInterface->GetHookPosition();
	//float LastPosAngle = FMath::Atan2(LastPosToCenter.Y, LastPosToCenter.X);

	// FVector LastPosToCenterNorm = LastPosToCenter.GetSafeNormal();
	// FVector NormalizedLastLocation = LastLocation.GetSafeNormal();
	// FVector NormalizeVelocity = Pawn->GetVelocity().GetSafeNormal();
	//
	// // (U.X * V.Y) - (U.Y * V.X)
	// float truc = (LastPosToCenterNorm.X * NormalizeVelocity.Y) - (LastPosToCenterNorm.Y * NormalizeVelocity.X);
	
	//float RotationDirection = truc > 0 ? 1.0f : -1.0f;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan,FString::Printf(TEXT("RotationDirection %f"), RotationDirection));


	TempGrapplingAngularVelocity = Pawn->BehaviorGrapple->StartGrapplingForceFactorWhenAlreadyMoving * RotationDirection
		* Pawn->BehaviorGrapple->GrapplingNotPillarForce * 1000.f;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Magenta,FString::Printf(TEXT("New Velocity %f"), TempGrapplingAngularVelocity));

	//TempGrapplingAngularVelocity = FMath::Clamp(TempGrapplingAngularVelocity, -10.0f, 10.0f); // valeurs temp

	TempGrapplingAngle = Pawn->BehaviorGrapple->CurrentGrapplingAngle + TempGrapplingAngularVelocity * DeltaTime;
}

bool UPlayerBallStateGrappling::DetectWalls()
{
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);

	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //
	// FVector SphereCenter = Pawn->BehaviorGrapple->GrappledPlayerBall->GetActorLocation() + Pawn->BehaviorGrapple->GrapplingOffset;
	// ---- OLD VERSION - GRAPPLING BETWEEN 2 PLAYERS ----- //

	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //
	FVector SphereCenter = Pawn->BehaviorGrapple->HookInterface->GetHookPosition() + Pawn->BehaviorGrapple->
		GrapplingOffset;
	// ----- NEW VERSION - GRAPPLING BETWEEN PLAYER AND HOOK POINT ----- //

	float SphereRadius = Pawn->SphereCollision->GetScaledSphereRadius() + 0.2f;

	FCollisionObjectQueryParams ObjectQueryParams(ECC_GameTraceChannel2); // Look only for walls

	// Detect Collision With sphere overlap
	bool bHasDetected = Pawn->GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		SphereCenter,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(SphereRadius),
		CollisionParams
	);

	/*
	// Debug collision sphere
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

	return bHasDetected;
}

void UPlayerBallStateGrappling::GainPillarPoints()
{
	if (ScoreSubsystem)
	{
		ScoreSubsystem->AddScore(Pawn->PlayerIndex,
		                         CurrentTimeOnPillar * Pawn->BehaviorGrapple->PillarPointsMultiplier * Pawn->
		                         BehaviorGrapple->PillarPointsPerSeconds);
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Pas trouvé de score subsystem");
	}
}
