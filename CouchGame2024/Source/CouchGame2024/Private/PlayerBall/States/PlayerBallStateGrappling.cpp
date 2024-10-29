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

	if (Pawn != nullptr)
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

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("c ciao"));

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

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, "tick");

	/*
	// Vérifier la direction de mouvement du joueur
	FVector MoveDirection = FVector(Pawn->GetActorLocation() - LastLocation).GetSafeNormal();
	FVector StartLocation = LastLocation;
	FVector EndLocation = StartLocation + MoveDirection * (Pawn->SphereCollision->GetScaledSphereRadius() + 10.f);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("WallTrace")), true, Pawn);
	TraceParams.bTraceComplex = true;
	TraceParams.AddIgnoredActor(Pawn);

	bool bHitWall = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel2, TraceParams);
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Green, false, 1, 0, 2.f);

	// Debug LineTrace
	if (bHitWall)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Wall detected in movement direction"));
		}

		// Arrêter le joueur s'il y a un mur dans la direction de mouvement
		Pawn->SphereCollision->SetPhysicsAngularVelocityInRadians(FVector::ZeroVector);
		return;
	}
	*/
	/*
	// ----- Check Walls ----- //
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);

	FCollisionObjectQueryParams ObjectQueryParams(ECC_GameTraceChannel2);    // Look only for walls

	// Detect Collision With sphere overlap
	bool bHasDetected = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Pawn->GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(Pawn->SphereCollision->GetScaledSphereRadius() + 0.2f),
		CollisionParams
	);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Is Detected: %hhd"), bHasDetected));
	
	if (bHasDetected) // rajouter && que le joueur pointe dans un direction ou y'a pas de mur
	{
		// Obtenir la direction de l'input du joueur (par exemple, sa direction de mouvement)
		FVector InputDirection = FVector(Pawn->MoveXValue, Pawn->MoveYValue, 0.f);

		// Vérifier si l'un des murs détectés est dans la même direction que l'input
		bool bWallInInputDirection = false;
		for (const FOverlapResult& Result : OverlapResults)
		{
			FVector WallDirection = (Result.GetActor()->GetActorLocation() - Pawn->GetActorLocation()).GetSafeNormal();
			float DotProduct = FVector::DotProduct(InputDirection, WallDirection);

			// Si le mur est dans la même direction que l'input (angle inférieur à 90°)
			if (DotProduct > 0.5f)  // 0.5 correspond environ à un angle de 60°
			{
				bWallInInputDirection = true;
				break;
			}
		}

		// Si un mur est dans la direction de l'input, on arrête le joueur
		if (bWallInInputDirection)
		{
			Pawn->SphereCollision->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
			return;
		}
	}
	*/
	// ----- Check Walls ----- //

	SetGrapplingVelocityAndAngle(GetWorld()->DeltaTimeSeconds);

	Pawn->GrapplingOffset = FVector(FMath::Cos(Pawn->CurrentGrapplingAngle), FMath::Sin(Pawn->CurrentGrapplingAngle), 0)
		* Pawn->CableLength;

	float DistanceBetweenGrappledPlayers = FVector::Dist(Pawn->GetActorLocation(),
	                                                     Pawn->GrappledPlayerBall->GetActorLocation());

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Distance: %f"), DistanceBetweenGrappledPlayers));

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("MoveX: %f"), Pawn->MoveXValue));

	FVector NormalizedDirection = (Pawn->GetActorLocation() - Pawn->GrappledPlayerBall->GetActorLocation()).
		GetSafeNormal();

	// More or less cord
	if (FMath::Abs(Pawn->MoveYValue) > 0.5f)
	{
		if (Pawn->MoveYValue > 0.f && DistanceBetweenGrappledPlayers > Pawn->MinCableDistance) // Less cord
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Less cord");

			Pawn->GrapplingOffset -= NormalizedDirection * Pawn->MoreOrLessCablePerFrame;
		}
		else if (Pawn->MoveYValue < 0.f && DistanceBetweenGrappledPlayers < Pawn->MaxCableDistance) // More cord
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "More cord");

			Pawn->GrapplingOffset += NormalizedDirection * Pawn->MoreOrLessCablePerFrame;
		}
	}

	//TObjectPtr<FHitResult> OutSweepHitResult;
	Pawn->SetActorLocation(Pawn->GrappledPlayerBall->GetActorLocation() + Pawn->GrapplingOffset);
	// Test de MoveComponent et AddForce -> ça fait nimp lol
	//Pawn->SetActorLocation(Pawn->GrappledPlayerBall->GetActorLocation() + Pawn->GrapplingOffset, true, OutSweepHitResult ); détecte bien les murs mais aussi les bumpers + un peu crade 
	Pawn->GrapplingCable->SetWorldLocation(Pawn->GetActorLocation());
	SetCable();

	//if (OutSweepHitResult.GetClass())

	LastLocation = Pawn->GetActorLocation();
}

/* -> Detecte bien les walls mais galere pour se debloquer si on bloque le joueur et pour détecter quand il met l'input dans un bon sens
	// ----- Check Walls ----- //
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(Pawn);

	FCollisionObjectQueryParams ObjectQueryParams(ECC_GameTraceChannel2);    // Look only for walls

	// Detect Collision With sphere overlap
	bool bHasDetected = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Pawn->GetActorLocation(),
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(Pawn->SphereCollision->GetScaledSphereRadius() + 0.2f),
		CollisionParams
	);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Is Detected: %hhd"), bHasDetected));
	
	if (bHasDetected) // rajouter && que le joueur pointe dans un direction ou y'a pas de mur
	{
		// Obtenir la direction de l'input du joueur (par exemple, sa direction de mouvement)
		FVector InputDirection = FVector(Pawn->MoveXValue, Pawn->MoveYValue, 0.f);

		// Vérifier si l'un des murs détectés est dans la même direction que l'input
		bool bWallInInputDirection = false;
		for (const FOverlapResult& Result : OverlapResults)
		{
			FVector WallDirection = (Result.GetActor()->GetActorLocation() - Pawn->GetActorLocation()).GetSafeNormal();
			float DotProduct = FVector::DotProduct(InputDirection, WallDirection);

			// Si le mur est dans la même direction que l'input (angle inférieur à 90°)
			if (DotProduct > 0.5f)  // 0.5 correspond environ à un angle de 60°
			{
				bWallInInputDirection = true;
				break;
			}
		}

		// Si un mur est dans la direction de l'input, on arrête le joueur
		if (bWallInInputDirection)
		{
			Pawn->SphereCollision->SetPhysicsAngularVelocityInRadians(FVector(0, 0, 0));
			return;
		}
	}
	*/
// ----- Check Walls ----- //


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
	Pawn->HookPoint = Pawn->GrappledPlayerBall->GetActorLocation();

	Pawn->CableLength = FVector::Dist(Pawn->GetActorLocation(), Pawn->HookPoint);

	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta,
	                                              FString::Printf(TEXT("Cable Length: %f"), Pawn->CableLength));
	Pawn->GrapplingCable->CableLength = Pawn->CableLength;
	Pawn->GrapplingCable->SetAttachEndToComponent(Pawn->GrappledPlayerBall->SphereMesh);
}

void UPlayerBallStateGrappling::SetGrapplingVelocityAndAngle(float DeltaTime)
{
	Pawn->CurrentGrapplingAngularVelocity = Pawn->GrapplingDamping * Pawn->CurrentGrapplingAngularVelocity + (Pawn->
			MoveXValue * -1 *
			Pawn->GrapplingForce) +
		Pawn->StartGrapplingForceFactorWhenAlreadyMoving * Pawn->GetVelocity().X;

	Pawn->CurrentGrapplingAngle += Pawn->CurrentGrapplingAngularVelocity * DeltaTime;
}
