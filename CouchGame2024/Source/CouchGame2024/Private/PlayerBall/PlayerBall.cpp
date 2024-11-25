// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBall.h"

#include "CableComponent.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraWorldSubsystem.h"
#include "Components/SphereComponent.h"
#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorMovements.h"
#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"
#include "PlayerBall/Datas/PlayerBallData.h"
#include "Rounds/RoundsSubsystem.h"
#include "Score/GlobalScoreSubsystem.h"


// Sets default values
APlayerBall::APlayerBall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	AttractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttractionSphere"));

	StartForceEffectWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("StartForceEffectWidget"));
	
	RootComponent = SphereCollision;
	SphereMesh->SetupAttachment(SphereCollision);
	AttractionSphere->SetupAttachment(SphereCollision);

	StartForceEffectWidget->SetupAttachment(SphereCollision);
	

	BehaviorMovements = CreateDefaultSubobject<UPlayerBallBehaviorMovements>(TEXT("BehaviorMovement"));
	BehaviorGrapple = CreateDefaultSubobject<UPlayerBallBehaviorGrapple>(TEXT("BehaviorGrapple"));
	BehaviorElementReactions = CreateDefaultSubobject<UPlayerBallBehaviorElementReactions>(TEXT("BehaviorElementReactions"));
	BehaviorPowerUp = CreateDefaultSubobject<UPlayerBallBehaviorPowerUp>(TEXT("BehaviorPowerUp"));
	

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

	InitResetable();
	InitLockableInput();
	InitFollowTarget();

	StartForceEffectWidget->SetHiddenInGame(true);
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


	// Respawn
	DeathDurationBeforeRespawn = PlayerBallData->DeathDurationBeforeRespawn;
	
}

TObjectPtr<UPlayerBallData> APlayerBall::GetPlayerBallData() const
{
	return PlayerBallData;
}

TObjectPtr<UPlayerPowerUpData> APlayerBall::GetPlayerPowerUpData() const
{
	return PlayerPowerUpData;
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
	StateMachine = NewObject<UPlayerBallStateMachine>(this);
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
	PlayerBallController->OnPlayerPunchInput.AddDynamic(this, &APlayerBall::ReceivePunchAction);

	if (BehaviorGrapple != nullptr)
	{
		BehaviorGrapple->BindBehaviorEventAction(PlayerBallController);
	}
	
	if (BehaviorPowerUp != nullptr)
	{
		BehaviorPowerUp->BindBehaviorEventAction(PlayerBallController);
	}
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

void APlayerBall::InitResetable()
{
	if (!GetWorld())	return;

	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr)	return;

	RoundsSubsystem->AddResetableObject(this);
}

bool APlayerBall::IsResetable()
{
	return true;
}

void APlayerBall::ResetObject()
{
	ResetState();
	ResetMovement();
	ResetGrapple();
	ResetCooldown();
	ResetPosition();
	ResetPhysics();
}

void APlayerBall::ResetState()
{
	if (StateMachine == nullptr)	return;

	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void APlayerBall::ResetMovement()
{
	this->GetVelocity() = FVector::ZeroVector;
	
	if (SphereCollision != nullptr)
	{
		SphereCollision->SetPhysicsLinearVelocity(FVector::ZeroVector);
		SphereCollision->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);
		SphereCollision->SetWorldRotation(FRotator::ZeroRotator);
		SphereCollision->ComponentVelocity = FVector::ZeroVector;
		PawnMovement->Velocity = FVector::ZeroVector;
	}
}

void APlayerBall::ResetGrapple()
{
	BehaviorGrapple->ResetGrapplingCooldown();
}

void APlayerBall::ResetCooldown()
{
	CurrentPunchCooldown = 0.f;
	bCanPunch = true;
}

void APlayerBall::ResetPosition()
{
	
}

void APlayerBall::ResetPhysics()
{
	if (SphereCollision == nullptr)	return;

	SphereCollision->SetSimulatePhysics(true);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
}

void APlayerBall::InitLockableInput()
{
	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr)	return;

	RoundsSubsystem->AddLockableInput(this);
}

bool APlayerBall::IsLockableInput()
{
	return true;
}

void APlayerBall::LockInput()
{
	
	bIsLocked = true;

	if (StateMachine == nullptr)	return;

	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Purple, TEXT("Lock player"));
	
	StateMachine->ChangeState(EPlayerBallStateID::Locked, 0.f);
}

void APlayerBall::UnlockInput()
{
	if (StateMachine == nullptr)	return;
	
	bIsLocked = false;
	bIsLockedButSpecial = false;

	StateMachine->ChangeState(EPlayerBallStateID::Idle);
}

void APlayerBall::LockButOnlySpecialInput()
{
	if (StateMachine == nullptr)	return;
	
	bIsLocked = true;
	bIsLockedButSpecial = true;

	StateMachine->ChangeState(EPlayerBallStateID::Locked, 1.f);
}

int APlayerBall::GetLockableInputIndex()
{
	return PlayerIndex;
}

bool APlayerBall::IsLocked()
{
	return bIsLocked;
}

void APlayerBall::Kill()
{
	if (bIsDead)	return;
	
	OnDeathReaction.Broadcast(1.f);
	ReceiveOnKill();
}

void APlayerBall::TestCallRespawn()
{
	Respawn();
}

void APlayerBall::Respawn()
{
	if (!bIsDead)	return;
	
	OnRespawnAction.Broadcast(1.f);
}

void APlayerBall::InitFollowTarget()
{
	if (GetWorld() == nullptr)	return;
	if (GetWorld()->GetSubsystem<UCameraWorldSubsystem>() == nullptr)	return;
	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
}

FVector APlayerBall::GetFollowPosition() const
{
	return GetActorLocation();
}

bool APlayerBall::IsFollowable() const
{
	return true;
}

void APlayerBall::ReceiveDuckReaction(int PlayerIndexReceiving, int PlayerIndexLosing)
{
	// Fonction pour ajouter duck ou retirer
	UGlobalScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	if (ScoreSubsystem != nullptr)
	{
		ScoreSubsystem->StealDuck(PlayerIndexReceiving, PlayerIndexLosing);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Receive Duck Reaction RECEIVING : %d	/	LOSING : %d"), PlayerIndexReceiving, PlayerIndexLosing));
}
