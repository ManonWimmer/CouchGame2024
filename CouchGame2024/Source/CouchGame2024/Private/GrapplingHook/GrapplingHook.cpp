// Fill out your copyright notice in the Description page of Project Settings.


#include "GrapplingHook/GrapplingHook.h"
#include "GrapplingHook/GrapplingHook.h"

#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "CableComponent.h"

// Sets default values
AGrapplingHook::AGrapplingHook()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));

	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	SphereMesh->SetupAttachment(RootComponent);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(SphereMesh);
	SphereCollision->SetCollisionProfileName(TEXT("GrapplingSphereCollision")); // Collisions

	Cable = CreateDefaultSubobject<UCableComponent>(TEXT("Cable"));
	Cable->SetupAttachment(SphereMesh);
	Cable->EndLocation = FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void AGrapplingHook::BeginPlay()
{
	Super::BeginPlay();

	SphereCollision->SetSphereRadius(MaxCableDistance); // Max cable distance
}

void AGrapplingHook::OnMove(FVector InputVector)
{
	if (!IsGrappling)
	{
		AddMovementInput(GetActorRightVector(), InputVector.X, false); // Left / Right
		AddMovementInput(GetActorForwardVector(), InputVector.Y, false); // Up / Down
	}
	else
	{
		MoveInputX = InputVector.X;
		MoveInputY = InputVector.Y;
	}
}

void AGrapplingHook::OnGrapplingStarted()
{
	IsGrappling = false;
	TArray<AActor*> OverlappingActors;
	SphereCollision->GetOverlappingActors(OverlappingActors, AGrapplingHook::StaticClass());

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor != this)
		{
			AGrapplingHook* OtherPlayer = Cast<AGrapplingHook>(Actor);
			if (OtherPlayer) // Later, add check if same team (2v2) ou take nearest (1v1v1v1)
			{
				GrappledPlayerBall = OtherPlayer;
				GrappledPlayerBall->GrapplingPlayerBall = this;
				IsGrappling = true;
			}
		}
	}
	
	if (IsGrappling)
	{
		Cable->SetHiddenInGame(false);
		SetCable();

		FVector Direction = GetActorLocation() - GrappledPlayerBall->GetActorLocation();
		CurrentGrapplingAngle = atan2f(Direction.Y, Direction.X);
		CurrentGrapplingAngularVelocity = 0.f;
	}
}

void AGrapplingHook::OnGrapplingCompleted()
{
	if (!IsGrappling) return;

	IsGrappling = false;

	LastAngle = CurrentGrapplingAngle;
	SetGrapplingVelocityAndAngle(GetWorld()->DeltaTimeSeconds);

	if (CurrentGrapplingAngle > LastAngle)
		AngleRotate = 90.f;
	else
		AngleRotate = -90.f;

	Cable->SetHiddenInGame(true);

	ReleaseDirection = GetActorLocation() + (GetActorLocation() - GrappledPlayerBall->GetActorLocation()).
		RotateAngleAxis(AngleRotate, FVector(1, 0, 0));
	ReleaseDirection.Z = 0.f;

	SphereMesh->AddImpulse(
		ReleaseDirection.GetSafeNormal(0.0001f) * FMath::Abs(CurrentGrapplingAngularVelocity) * GrapplingReleaseForce,
		"None", true);

	CurrentGrapplingAngle = 0.f;
	CurrentGrapplingAngularVelocity = 0.f;
}

void AGrapplingHook::OnGrapplingTriggered()
{
	if (!IsGrappling) return;
	
	SetGrapplingVelocityAndAngle(GetWorld()->DeltaTimeSeconds);

	GrapplingOffset = FVector(FMath::Cos(CurrentGrapplingAngle), FMath::Sin(CurrentGrapplingAngle), 0) * CableLength;

	float DistanceBetweenGrappledPlayers = FVector::Dist(GetActorLocation(), GrappledPlayerBall->GetActorLocation());

	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, FString::Printf(TEXT("Distance: %f"), DistanceBetweenGrappledPlayers));
	
	FVector NormalizedDirection = (GetActorLocation() - GrappledPlayerBall->GetActorLocation()).GetSafeNormal();
	
	// More or less cord
	if (FMath::Abs(MoveInputY) > 0.5f)
	{
		if (MoveInputY > 0.f && DistanceBetweenGrappledPlayers > MinCableDistance) // Less cord
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Less cord");
	
			GrapplingOffset -= NormalizedDirection * MoreOrLessCablePerFrame;
		}
		else if (MoveInputY < 0.f && DistanceBetweenGrappledPlayers < MaxCableDistance) // More cord
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "More cord");
	
			GrapplingOffset += NormalizedDirection * MoreOrLessCablePerFrame;
			
		}
	}
	
	SetActorLocation(GrappledPlayerBall->GetActorLocation() + GrapplingOffset);
	Cable->SetWorldLocation(GetActorLocation());
	SetCable();
}

void AGrapplingHook::SetCable()
{
	Cable->SetWorldLocation(GetActorLocation());
	HookPoint = GrappledPlayerBall->GetActorLocation();
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, HookPoint.ToString());
	
	CableLength = FVector::Dist(GetActorLocation(), HookPoint);

	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Magenta, FString::Printf(TEXT("Cable Length: %f"), CableLength));
	Cable->CableLength = CableLength;
	Cable->SetAttachEndToComponent(GrappledPlayerBall->SphereMesh);
}

void AGrapplingHook::SetGrapplingVelocityAndAngle(float DeltaTime)
{
	CurrentGrapplingAngularVelocity = GrapplingDamping * CurrentGrapplingAngularVelocity + (MoveInputX * -1 *
			GrapplingForce) +
		StartGrapplingForceFactorWhenAlreadyMoving * GetVelocity().X;

	CurrentGrapplingAngle += CurrentGrapplingAngularVelocity * DeltaTime;
}

// Called every frame
void AGrapplingHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGrapplingHook::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
