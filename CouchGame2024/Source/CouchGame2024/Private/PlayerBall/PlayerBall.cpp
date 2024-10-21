// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/PlayerBall/PlayerBall.h"

#include "Components/SphereComponent.h"
#include "CouchGame2024/Public/PlayerBall/PlayerBallController.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"

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
}

// Called when the game starts or when spawned
void APlayerBall::BeginPlay()
{
	Super::BeginPlay();
	
	BindEventActions();
}

// Called every frame
void APlayerBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PawnMovement->Velocity.Length() >= 10.f)
	{
		UpdateRotation();
	}
}

// Called to bind functionality to input
void APlayerBall::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerBall::BindEventActions()
{
	if (Controller == nullptr)
		return;
	
	APlayerBallController* BallController = Cast<APlayerBallController>(Controller);
	
	if (BallController == nullptr)
		return;
	
	BallController->OnPlayerMoveX.AddDynamic(this, &APlayerBall::MoveXAction);
}

void APlayerBall::MoveXAction(float XValue)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("MoveXAction")
		);
	
	if (PawnMovement == nullptr)
		return;

	FVector RightVect = GetActorRightVector();

	FVector Dir = RightVect * XValue;
	
	PawnMovement->AddInputVector(Dir);
}

void APlayerBall::MoveYAction(float YValue)
{
	
}

void APlayerBall::UpdateRotation()
{
	if (PawnMovement == nullptr)
		return;

	FVector TempVel = PawnMovement->Velocity;

	FRotator TempRot(0.f, 0.f, TempVel.Y);
	
	SphereMesh->AddLocalRotation(TempRot * GetWorld()->GetDeltaSeconds());
}

