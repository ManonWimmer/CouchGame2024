// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBall.h"

#include "CableComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerState.h"
#include "PinballElements/PinballElement.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Datas/PlayerBallData.h"
#include "PowerUp/PowerUp.h"

void APlayerBall::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr) return;

	TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

	if (OtherBall != nullptr)
	{
		ImpactedPlayerBall = OtherBall;
		ReceiveImpactAction(1.f, Hit.ImpactNormal);
	}
	else
	{
		TObjectPtr<APinballElement> OtherElement = Cast<APinballElement>(OtherActor);

		if (OtherElement != nullptr)
		{
			switch (OtherElement->GetElementID())
			{
			case EPinballElementID::Bumper:
				OtherElement->TriggerElement();
				ReceiveBumperReaction(OtherElement, Hit.ImpactNormal);
				return;
			case EPinballElementID::Flipper:
				return;
			case EPinballElementID::None:
				return;

			default:
				return;
			}
		}
	}
}

void APlayerBall::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "OnBeginOverlap");

	TObjectPtr<APowerUp> OtherPowerUp = Cast<APowerUp>(OtherActor);

	if (OtherPowerUp != nullptr)
	{
		switch (OtherPowerUp->GetPowerUpID()) {
		case EPowerUpID::None:
			break;
		case EPowerUpID::Dash:
			OtherPowerUp->TriggerPowerUp();
			break;
		case EPowerUpID::Collectible:
			OtherPowerUp->TriggerPowerUp(PlayerIndex);
			break;

		default:
			break;
		}

		return;
	}
}

void APlayerBall::OnAttractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (SnappingPlayerBall == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "OnAttractionBeginOverlap");

		
		TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

		if (OtherBall != nullptr)
		{
			SnappingPlayerBall = OtherBall;
			ReceiveSnappingAction(1.f);

			return;
		}
	}
}

void APlayerBall::OnAttractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (SnappingPlayerBall == nullptr)	return;

	TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

	if (OtherBall != nullptr)
	{
		if (OtherBall ==  SnappingPlayerBall)
		{
			ReceiveSnappingAction(0.f);
		}
	}
}

// Sets default values
APlayerBall::APlayerBall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	AttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionSphere"));
	
	SphereCollision->SetupAttachment(RootComponent);
	SphereMesh->SetupAttachment(SphereCollision);
	AttractionSphere->SetupAttachment(SphereCollision);

	if (SphereCollision != nullptr)
	{
		SphereCollision->OnComponentHit.AddDynamic(this, &APlayerBall::OnCollisionHit);
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerBall::OnBeginOverlap);
	}
	if (AttractionSphere != nullptr)
	{
		AttractionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerBall::OnAttractionBeginOverlap);
		AttractionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerBall::OnAttractionEndOverlap);
	}

	// ----- Setup Grappling ----- //
	GrapplingSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("GrapplingSphereCollision"));
	GrapplingSphereCollision->SetupAttachment(SphereCollision);
	GrapplingSphereCollision->SetCollisionProfileName(TEXT("GrapplingSphereCollision")); // Collisions

	GrapplingCable = CreateDefaultSubobject<UCableComponent>(TEXT("GrapplingCable"));
	GrapplingCable->SetupAttachment(SphereCollision);
	GrapplingCable->EndLocation = FVector(0, 0, 0);
	// ----- Setup Grappling ----- //
}

// Called when the game starts or when spawned
void APlayerBall::BeginPlay()
{
	Super::BeginPlay();

	CreateStateMachine();
	InitStateMachine();

	SetupData();
}

// Called every frame
void APlayerBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Current state: %hhd"), StateMachine->GetCurrentStateID()));
}

// Called to bind functionality to input
void APlayerBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	BindEventActions();
}

void APlayerBall::SetupData() // Get all data and set them
{
	if (PlayerBallData == nullptr)
		return;

	if (PawnMovement == nullptr)
		return;

	if (SphereCollision == nullptr)
		return;

	// Movements
	AngularRollForce = PlayerBallData->AngularRollForce;
	BraqueDirectionForceMultiplier = PlayerBallData->BraqueDirectionForceMultiplier;
	SphereCollision->SetAngularDamping(PlayerBallData->AngularRollDamping);
	SphereCollision->SetPhysicsMaxAngularVelocityInDegrees(PlayerBallData->MaxAngularRollVelocity);

#pragma region FallData (Obsolete)
	/*
	// Fall Obsolete
	PawnMovement->Acceleration = PlayerBallData->AirControlSideAcceleration;
	PawnMovement->MaxSpeed = PlayerBallData->AirControlSideMaxSpeed;
	PawnMovement->Deceleration = PlayerBallData->AirControlSideDeceleration;
	SlowFallForce = PlayerBallData->SlowFallForce;
	AccelerateFallForce = PlayerBallData->AccelerateFallForce;
	*/
#pragma endregion 
	
	// Stun By punch
	PunchStunCooldown = PlayerBallData->PunchStunCooldown;

	// Punch
	PunchCooldown = PlayerBallData->PunchCooldown;
	PunchRadius = PlayerBallData->PunchRadius;
	PunchForceMultiplier = PlayerBallData->PunchForceMultiplier;
	
	// Impact
	ImpactForceMultiplier = PlayerBallData->ImpactForceMultiplier;
	ImpactMinTotalForce = PlayerBallData->ImpactMinTotalForce;
	ImpactStunCooldown = PlayerBallData->ImpactStunCooldown;

	// Bumped
	BumpedForceMultiplier = PlayerBallData->BumpedForceMultiplier;
	BumpedHitLagCooldown = PlayerBallData->BumpedHitLagStunCooldown;

	// Snapping
	SnapAngularForce = PlayerBallData->SnapAngularForce;
	SnapControlMoveRollDivider = PlayerBallData->SnapControlMoveRollDivider;
	MinVelocityToSnap = PlayerBallData->MinVelocityToSnap;
	if (PlayerBallData->SnapTriggerRadius == PlayerBallData->MaxCableDistance)	// to avoid a case in which 2 spheres overlap and causes physics bug
	{
		AttractionSphere->SetSphereRadius(PlayerBallData->SnapTriggerRadius - 2.f);
	}
	else
	{
		AttractionSphere->SetSphereRadius(PlayerBallData->SnapTriggerRadius);
	}

	// Grappling
	GrapplingDamping = PlayerBallData->GrapplingDamping;
	GrapplingForce = PlayerBallData->GrapplingForce;
	GrapplingReleaseForce = PlayerBallData->GrapplingReleaseForce;
	MinCableDistance = PlayerBallData->MinCableDistance;
	MaxCableDistance = PlayerBallData->MaxCableDistance;
	MoreOrLessCablePerFrame = PlayerBallData->MoreOrLessCablePerFrame;
	StartGrapplingForceFactorWhenAlreadyMoving = PlayerBallData->StartGrapplingForceFactorWhenAlreadyMoving;
	GrapplingSphereCollision->SetSphereRadius(MaxCableDistance); // Max grappling cable distance
}


void APlayerBall::CreateStateMachine() // Create a StateMachine Object for the Pawn
{
	StateMachine = NewObject<UPlayerBallStateMachine>();
}

void APlayerBall::InitStateMachine() // Call Init on StateMachine
{
	if (StateMachine == nullptr) return;

	StateMachine->Init(this);
}

void APlayerBall::TickStateMachine(float DeltaTime) const // Call tick on StateMachine
{
	if (StateMachine == nullptr) return;

	StateMachine->Tick(DeltaTime);
}

void APlayerBall::BindEventActions() // Bind Input Event from controller to Pawn Actions
{
	if (Controller == nullptr)
		return;

	APlayerBallController* BallController = Cast<APlayerBallController>(Controller);

	if (BallController == nullptr)
		return;

	BallController->OnPlayerMoveXInput.AddDynamic(this, &APlayerBall::MoveXAction);
	BallController->OnPlayerMoveYInput.AddDynamic(this, &APlayerBall::MoveYAction);
	BallController->OnPlayerPunchInput.AddDynamic(this, &APlayerBall::ReceivePunchAction);
	BallController->OnPlayerGrapplingInputStarted.AddDynamic(this, &APlayerBall::ReceiveGrapplingActionStarted);
	BallController->OnPlayerGrapplingInputEnded.AddDynamic(this, &APlayerBall::ReceiveGrapplingActionEnded);
	BallController->OnPlayerMoreLessGrapplingInput.AddDynamic(this,&APlayerBall::MoreLessAction);
}

bool APlayerBall::IsGrounded()
{
	// Line trace Pos
	FVector Start = GetActorLocation();
	FVector End = Start + FVector(0, 0, -50.f);

	// LineTrace Parameters
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	// Line Trace Hit
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_WorldStatic,
		QueryParams
	);

	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);

	return bHit;
}

void APlayerBall::MoveXAction(float XValue) // Set MoveX Value
{
	MoveXValue = XValue;
}

void APlayerBall::MoveYAction(float YValue) //Set MoveY Value
{
	MoveYValue = YValue;
}

void APlayerBall::ReceiveStunnedAction(float InStunnedDurationValue)
{
	OnStunnedAction.Broadcast(InStunnedDurationValue);
}

void APlayerBall::ReceivePunchAction(float InPunchValue)
{
	OnPunchAction.Broadcast(InPunchValue);
}

void APlayerBall::ReceiveImpactAction(float ImpactValue, const FVector &InNormalImpact)
{
	NormalImpact = InNormalImpact;

	NormalImpact.Z = 0.f;

	NormalImpact.Normalize();
	
	OnImpactAction.Broadcast(ImpactValue);
}

void APlayerBall::ReceiveBumperReaction(APinballElement* Element, const FVector &InNormalBump)
{
	HitPinballElement = Element;

	NormalBump = InNormalBump;

	NormalBump.Z = 0.f;
	NormalBump.Normalize();

	OnBumperReaction.Broadcast(1.f);
}
/*
void APlayerBall::ReceiveGrapplingAction(float InGrapplingValue)
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "grappling action");

	if (StateMachine->GetCurrentStateID() == EPlayerBallStateID::Stun)
		return;
	
	if (InGrapplingValue == 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "cc");
		OnGrapplingAction.Broadcast(InGrapplingValue);
		return;
	}
	
	GrapplingValue = InGrapplingValue;

	// Check for nearest in grappling radius
	IsGrappling = false;
	TArray<TObjectPtr<AActor>> OverlappingActors;
	TArray<TObjectPtr<APlayerBall>> OverlappingPlayers;
	SphereCollision->GetOverlappingActors(OverlappingActors, APlayerBall::StaticClass());

	if (OverlappingActors.Num() <= 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "pas de joueur");
	
		return;
	}

	for (TObjectPtr<AActor> Actor : OverlappingActors)
	{
		if (Actor != this)
		{
			TObjectPtr<APlayerBall> OtherPlayer = Cast<APlayerBall>(Actor);
			if (OtherPlayer) 
			{
				if (OtherPlayer->GrappledPlayerBall == nullptr && OtherPlayer->GrapplingPlayerBall == nullptr)
					OverlappingPlayers.Add(OtherPlayer);
			}
		}
	}

	if (OverlappingPlayers.Num() <= 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "pas de joueur 2");
	
		return;
	}

	TObjectPtr<APlayerBall> NearestPlayerBall = OverlappingPlayers[0];
	float NearestDistance = FVector::Dist(GetActorLocation(), NearestPlayerBall->GetActorLocation());

	for (APlayerBall* PlayerBall : OverlappingPlayers)
	{
		float NewNearestDistance = FVector::Dist(GetActorLocation(), PlayerBall->GetActorLocation());
		if (NewNearestDistance < NearestDistance)
		{
			NearestPlayerBall = PlayerBall;
			NearestDistance = NewNearestDistance;
		}
	}

	GrappledPlayerBall = NearestPlayerBall;
	GrappledPlayerBall->GrapplingPlayerBall = this;
	IsGrappling = true;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "oui");
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "broadcast grappling");

	if (GrappledPlayerBall != nullptr)
		OnGrapplingAction.Broadcast(GrapplingValue);
	
}
*/
void APlayerBall::ReceiveGrapplingActionStarted(float InGrapplingValue)
{
	if (!CanGrappling)
		return;
	
	if (InGrapplingValue == 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "cc");
		OnGrapplingActionEnded.Broadcast(InGrapplingValue);
		return;
	}
	
	GrapplingValue = InGrapplingValue;

	// Check for nearest in grappling radius
	IsGrappling = false;
	TArray<TObjectPtr<AActor>> OverlappingActors;
	TArray<TObjectPtr<APlayerBall>> OverlappingPlayers;
	SphereCollision->GetOverlappingActors(OverlappingActors, APlayerBall::StaticClass());

	if (OverlappingActors.Num() <= 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "pas de joueur");
	
		return;
	}

	for (TObjectPtr<AActor> Actor : OverlappingActors)
	{
		if (Actor != this)
		{
			TObjectPtr<APlayerBall> OtherPlayer = Cast<APlayerBall>(Actor);
			if (OtherPlayer) 
			{
				if (OtherPlayer->GrappledPlayerBall == nullptr && OtherPlayer->GrapplingPlayerBall == nullptr && OtherPlayer->CanBeGrappled)
					OverlappingPlayers.Add(OtherPlayer);
			}
		}
	}

	if (OverlappingPlayers.Num() <= 0)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "pas de joueur 2");
	
		return;
	}

	TObjectPtr<APlayerBall> NearestPlayerBall = OverlappingPlayers[0];
	float NearestDistance = FVector::Dist(GetActorLocation(), NearestPlayerBall->GetActorLocation());

	for (APlayerBall* PlayerBall : OverlappingPlayers)
	{
		float NewNearestDistance = FVector::Dist(GetActorLocation(), PlayerBall->GetActorLocation());
		if (NewNearestDistance < NearestDistance)
		{
			NearestPlayerBall = PlayerBall;
			NearestDistance = NewNearestDistance;
		}
	}

	GrappledPlayerBall = NearestPlayerBall;
	GrappledPlayerBall->GrapplingPlayerBall = this;
	IsGrappling = true;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, "oui");
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "broadcast grappling");

	if (GrappledPlayerBall != nullptr)
		OnGrapplingActionStarted.Broadcast(GrapplingValue);
}

void APlayerBall::ReceiveGrapplingActionEnded(float InGrapplingValue)
{
	OnGrapplingActionEnded.Broadcast(InGrapplingValue);
}

void APlayerBall::ReceiveGrappledActionStarted(float InGrappledValue) // 0 -> end grappled	1 -> start grappled
{
	OnGrappledActionStarted.Broadcast(InGrappledValue);
}

void APlayerBall::ReceiveGrappledActionEnded(float InGrappledValue)
{
	OnGrappledActionEnded.Broadcast(InGrappledValue);
}

void APlayerBall::ReceiveSnappingAction(float SnappingValue)
{
	OnReceiveSnappingAction.Broadcast(SnappingValue);
}

void APlayerBall::MoreLessAction(float InMoreLessValue)
{
	MoreLessValue = InMoreLessValue;
}
