// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBall.h"

#include "CableComponent.h"
#include "EnhancedInputComponent.h"
#include "Components/SphereComponent.h"
#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "PinballElements/PinballElement.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"
#include "PlayerBall/Datas/PlayerBallData.h"
#include "PowerUp/PowerUp.h"

void APlayerBall::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                 FVector NormalImpulse, const FHitResult& Hit)
{
	/*
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
	*/
}

void APlayerBall::OnAttractionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	if (SnappingPlayerBall == nullptr)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "OnAttractionBeginOverlap");

		
		TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

		if (OtherBall != nullptr)
		{
			SnappingPlayerBall = OtherBall;
			ReceiveSnappingAction(1.f);

			return;
		}
	}
	*/
}

void APlayerBall::OnAttractionEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*
	if (SnappingPlayerBall == nullptr)	return;

	TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

	if (OtherBall != nullptr)
	{
		if (OtherBall ==  SnappingPlayerBall)
		{
			ReceiveSnappingAction(0.f);
		}
	}
	*/
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
	
	RootComponent = SphereCollision;
	SphereMesh->SetupAttachment(SphereCollision);
	AttractionSphere->SetupAttachment(SphereCollision);

	BehaviorMovements = CreateDefaultSubobject<UPlayerBallBehaviorMovements>(TEXT("BehaviorMovement"));
	BehaviorGrapple = CreateDefaultSubobject<UPlayerBallBehaviorGrapple>(TEXT("BehaviorGrapple"));
	BehaviorElementReactions = CreateDefaultSubobject<UPlayerBallBehaviorElementReactions>(TEXT("BehaviorElementReactions"));
	BehaviorPowerUp = CreateDefaultSubobject<UPlayerBallBehaviorPowerUp>(TEXT("BehaviorPowerUp"));
	
	if (SphereCollision != nullptr)
	{
		//SphereCollision->OnComponentHit.AddDynamic(this, &APlayerBall::OnCollisionHit);
	}
	if (AttractionSphere != nullptr)
	{
		//AttractionSphere->OnComponentBeginOverlap.AddDynamic(this, &APlayerBall::OnAttractionBeginOverlap);
		//AttractionSphere->OnComponentEndOverlap.AddDynamic(this, &APlayerBall::OnAttractionEndOverlap);
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

	InitPlayerBallBehaviors();
	
	SetupData();
}

// Called every frame
void APlayerBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);

	HandlePunchCooldown(DeltaTime);
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

#pragma region States Data

	

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

	// Punch
	PunchCooldown = PlayerBallData->PunchCooldown;
	PunchRadius = PlayerBallData->PunchRadius;
	PunchForceMultiplier = PlayerBallData->PunchForceMultiplier;

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

	
#pragma endregion
}

TObjectPtr<UPlayerBallData> APlayerBall::GetPlayerBallData() const
{
	return PlayerBallData;
}


void APlayerBall::InitPlayerBallBehaviors() const
{
	
	
	if (BehaviorMovements)
		BehaviorMovements->InitBehavior();

	if (BehaviorGrapple)
		BehaviorGrapple->InitBehavior();

	if (BehaviorElementReactions)
		BehaviorElementReactions->InitBehavior();

	if (BehaviorPowerUp)
		BehaviorPowerUp->InitBehavior();
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

	PlayerBallController = Cast<APlayerBallController>(Controller);

	if (PlayerBallController == nullptr)
		return;

	if (BehaviorMovements != nullptr)
	{
		BehaviorMovements->BindBehaviorEventAction(PlayerBallController);
	}
	//PlayerBallController->OnPlayerMoveXInput.AddDynamic(this, &APlayerBall::MoveXAction);
	//PlayerBallController->OnPlayerMoveYInput.AddDynamic(this, &APlayerBall::MoveYAction);
	PlayerBallController->OnPlayerPunchInput.AddDynamic(this, &APlayerBall::ReceivePunchAction);
	PlayerBallController->OnPlayerGrapplingInputStarted.AddDynamic(this, &APlayerBall::ReceiveGrapplingActionStarted);
	PlayerBallController->OnPlayerGrapplingInputEnded.AddDynamic(this, &APlayerBall::ReceiveGrapplingActionEnded);
	PlayerBallController->OnPlayerMoreLessGrapplingInput.AddDynamic(this,&APlayerBall::MoreLessAction);

	if (BehaviorPowerUp != nullptr)
	{
		BehaviorPowerUp->BindBehaviorEventAction(PlayerBallController);
	}
	//PlayerBallController->OnUsePowerUpInput.AddDynamic(this, &APlayerBall::UsePowerUpAction);
}


void APlayerBall::ReceivePunchAction(float InPunchValue)
{
	if (!bCanPunch)	return;
	
	OnPunchAction.Broadcast(InPunchValue);
}

void APlayerBall::HandlePunchCooldown(float DeltaTime)
{
	if (bCanPunch)	return;
	
	if (CurrentPunchCooldown > 0.f)
	{
		CurrentPunchCooldown -= DeltaTime;
	}
	else
	{
		bCanPunch = true;
	}
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

	if (GrappledPlayerBall == nullptr)
		return;
	
	// UE_LOG(LogTemp, Warning, TEXT("Set grappledPlayerBall") );
	// UE_LOG(LogTemp, Log, TEXT("Current State : %hhd"), (StateMachine->GetCurrentStateID()) );
	
	GrappledPlayerBall->GrapplingPlayerBall = this;
	
	// UE_LOG(LogTemp, Warning, TEXT("Set grapplingPlayerBall") );
	// UE_LOG(LogTemp, Log, TEXT("Current State : %hhd"), (StateMachine->GetCurrentStateID()) );
	
	IsGrappling = true;

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, "broadcast grappling");

	OnGrapplingActionStarted.Broadcast(GrapplingValue);
}

void APlayerBall::ReceiveGrapplingActionEnded(float InGrapplingValue)
{
	OnGrapplingActionEnded.Broadcast(InGrapplingValue);
}

void APlayerBall::ReceiveGrappledActionStarted(float InGrappledValue) // 0 -> end grappled	1 -> start grappled
{
	if (!CanBeGrappled)	return;
	
	OnGrappledActionStarted.Broadcast(InGrappledValue);
}

void APlayerBall::ReceiveGrappledActionEnded(float InGrappledValue)
{
	OnGrappledActionEnded.Broadcast(InGrappledValue);
}
void APlayerBall::MoreLessAction(float InMoreLessValue)
{
	MoreLessValue = InMoreLessValue;
}
