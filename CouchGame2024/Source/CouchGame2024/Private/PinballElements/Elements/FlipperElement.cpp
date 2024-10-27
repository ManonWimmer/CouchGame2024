// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/FlipperElement.h"

#include "Components/BoxComponent.h"


// Sets default values
AFlipperElement::AFlipperElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FlipperBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trigger"));
	FlipperMeshRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Root"));
	FlipperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	FlipperBoxTrigger->SetupAttachment(RootComponent);
	FlipperMeshRoot->SetupAttachment(FlipperBoxTrigger);
	FlipperMesh->SetupAttachment(FlipperMeshRoot);
}

// Called when the game starts or when spawned
void AFlipperElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlipperElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AFlipperElement::TriggerElement()
{
	Super::TriggerElement();

	ActivateFlip();
}

EPinballElementID AFlipperElement::GetElementID()
{
	return EPinballElementID::Flipper;
}

void AFlipperElement::ActivateFlip()
{
	if (IsFlipping)	return;

	IsFlipping = true;
}

void AFlipperElement::HandleFlipRotation(float DeltaTime)
{
	if (IsFlipping)
	{
		if (CurrentRotationAngle < FlipperMaxRotationAngle)
		{
			// Rotate the flipper towards the maximum angle

			CurrentRotationAngle = FMath::FInterpTo(CurrentRotationAngle, FlipperMaxRotationAngle, DeltaTime, RotationSpeed);
			
			CurrentRotationAngle = FMath::Clamp(CurrentRotationAngle, 0.0f, FlipperMaxRotationAngle);

			// Apply the rotation to the flipper mesh
			FRotator NewRotation = FRotator(CurrentRotationAngle, 0, 0);
			FlipperMeshRoot->SetRelativeRotation(NewRotation);
		}
	}
	else
	{
		CurrentRotationAngle = FMath::FInterpTo(CurrentRotationAngle, 0.f, DeltaTime, RotationSpeed);
			
		CurrentRotationAngle = FMath::Clamp(CurrentRotationAngle, 0.0f, FlipperMaxRotationAngle);

		// Apply the rotation to the flipper mesh
		FRotator NewRotation = FRotator(CurrentRotationAngle, 0, 0);
		FlipperMeshRoot->SetRelativeRotation(NewRotation);
	}
}

