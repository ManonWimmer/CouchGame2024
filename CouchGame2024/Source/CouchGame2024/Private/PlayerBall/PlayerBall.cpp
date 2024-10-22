// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBall.h"

#include "Components/SphereComponent.h"
#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerBall/PlayerBallStateMachine.h"
#include "PlayerBall/Datas/PlayerBallData.h"

void APlayerBall::OnCollisionHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == nullptr)	return;

	TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);
	
	if (OtherBall == nullptr)	return;

	ImpactedPlayerBall = OtherBall;

	ReceiveImpactAction(1.f);
}

// Sets default values
APlayerBall::APlayerBall()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereComponent"));
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	SphereCollision->SetupAttachment(RootComponent);
	SphereMesh->SetupAttachment(SphereCollision);


	if (SphereCollision != nullptr)
	{
		SphereCollision->OnComponentHit.AddDynamic(this, &APlayerBall::OnCollisionHit);
	}
}

// Called when the game starts or when spawned
void APlayerBall::BeginPlay()
{
	Super::BeginPlay();

	CreateStateMachine();
	InitStateMachine();
	
	BindEventActions();

	SetupData();
}

// Called every frame
void APlayerBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickStateMachine(DeltaTime);
}

// Called to bind functionality to input
void APlayerBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerBall::SetupData()	// Get all data and set them
{
	if (PlayerBallData == nullptr)
		return;

	if (PawnMovement == nullptr)
		return;

	if (SphereCollision == nullptr)
		return;

	AngularRollForce = PlayerBallData->AngularRollForce;
	BraqueDirectionForceMultiplier = PlayerBallData->BraqueDirectionForceMultiplier;
	SphereCollision->SetAngularDamping(PlayerBallData->AngularRollDamping);
	SphereCollision->SetPhysicsMaxAngularVelocityInDegrees(PlayerBallData->MaxAngularRollVelocity);

	PawnMovement->Acceleration = PlayerBallData->AirControlSideAcceleration;
	PawnMovement->MaxSpeed = PlayerBallData->AirControlSideMaxSpeed;
	PawnMovement->Deceleration = PlayerBallData->AirControlSideDeceleration;
	//SphereCollision->SetMassScale(NAME_None, PlayerBallData->GravityScale);

	StunCooldown = PlayerBallData->StunCooldown;

	PunchCooldown = PlayerBallData->PunchCooldown;
	PunchRadius = PlayerBallData->PunchRadius;
	PunchForceMultiplier = PlayerBallData->PunchForceMultiplier;

	ImpactForceMultiplier = PlayerBallData->ImpactForceMultiplier;
}


void APlayerBall::CreateStateMachine()	// Create a StateMachine Object for the Pawn
{
	StateMachine = NewObject<UPlayerBallStateMachine>();
}

void APlayerBall::InitStateMachine()	// Call Init on StateMachine
{
	if (StateMachine == nullptr)	return;

	StateMachine->Init(this);
}

void APlayerBall::TickStateMachine(float DeltaTime) const	// Call tick on StateMachine
{
	if (StateMachine == nullptr)	return;

	StateMachine->Tick(DeltaTime);
}

void APlayerBall::BindEventActions()	// Bind Input Event from controller to Pawn Actions
{
	if (Controller == nullptr)
		return;
	
	APlayerBallController* BallController = Cast<APlayerBallController>(Controller);
	
	if (BallController == nullptr)
		return;
	
	BallController->OnPlayerMoveXInput.AddDynamic(this, &APlayerBall::MoveXAction);
	BallController->OnPlayerPunchInput.AddDynamic(this, &APlayerBall::ReceivePunchAction);
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

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1.0f, 0, 1.0f);

	return bHit;
}

void APlayerBall::MoveXAction(float XValue)	// Set MoveX Value
{
	MoveXValue = XValue;
}

void APlayerBall::MoveYAction(float YValue)	//Set MoveY Value
{
	MoveYValue = YValue;
}

void APlayerBall::ReceiveStunnedAction(float InStunnedValue)
{
	OnStunnedAction.Broadcast(InStunnedValue);
}

void APlayerBall::ReceivePunchAction(float InPunchValue)
{
	OnPunchAction.Broadcast(InPunchValue);
}

void APlayerBall::ReceiveImpactAction(float ImpactValue)
{
	OnImpactAction.Broadcast(ImpactValue);
}

