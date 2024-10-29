// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/FlipperElement.h"

#include "Components/BoxComponent.h"
#include "PlayerBall/PlayerBall.h"


void AFlipperElement::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	TObjectPtr<APlayerBall> OtherBall = Cast<APlayerBall>(OtherActor);

	if (OtherBall != nullptr)
	{
		// Flip if not retourné, sinon reviens en position normale
		FlipUp();
	}
}

// Sets default values
AFlipperElement::AFlipperElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	FlipperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlipperMesh"));

	BoxTrigger->SetupAttachment(RootComponent);
	FlipperMesh->SetupAttachment(BoxTrigger);

	if (BoxTrigger != nullptr)
	{
		BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFlipperElement::OnTriggerBeginOverlap);
	}
}


// Called when the game starts or when spawned
void AFlipperElement::BeginPlay()
{
	Super::BeginPlay();

	// Rot Init
	OriginalRotation = FRotator(0.f, 0.f, 0.f);
	FlippedRotation = FRotator(0.f, 0.f, 45.f); // 45 deg
	bIsFlipped = false;

	OriginalRotation = FlipperMesh->GetRelativeRotation();
}

// Called every frame
void AFlipperElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFlipped &&  FlipperMesh->GetRelativeRotation() != FlippedRotation)
	{
		FlipperMesh->SetRelativeRotation(FMath::RInterpTo(FlipperMesh->GetRelativeRotation(), FlippedRotation, DeltaTime, FlipUpSpeed));
	}
	else if (!bIsFlipped && FlipperMesh->GetRelativeRotation() != OriginalRotation)
	{
		FlipperMesh->SetRelativeRotation(FMath::RInterpTo(FlipperMesh->GetRelativeRotation(), OriginalRotation, DeltaTime, FlipDownSpeed));

	}
}

EPinballElementID AFlipperElement::GetElementID()
{
	return EPinballElementID::Flipper;
}

void AFlipperElement::TriggerElement()
{
	Super::TriggerElement();

}

void AFlipperElement::FlipUp()
{
	if (bIsFlipped)	return;

	bIsFlipped = true;
}

void AFlipperElement::FlipDown()
{
	if (!bIsFlipped)	return;

	bIsFlipped = false;
}

