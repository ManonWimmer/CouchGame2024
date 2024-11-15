// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/RailElement.h"

#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"


// Sets default values
ARailElement::ARailElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SplineRail = CreateDefaultSubobject<USplineComponent>("SplineRail");
	SphereTriggerRail = CreateDefaultSubobject<USphereComponent>("SphereTriggerRail");
	SphereTriggerRailInverse = CreateDefaultSubobject<USphereComponent>("SphereTriggerRailInverse");

	RootComponent = RootSceneComp;

	SplineRail->SetupAttachment(RootSceneComp);
	SphereTriggerRail->SetupAttachment(SplineRail);
	SphereTriggerRailInverse->SetupAttachment(SplineRail);
}

// Called when the game starts or when spawned
void ARailElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARailElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPinballElementID ARailElement::GetElementID()
{
	return EPinballElementID::Rail;
}

void ARailElement::TriggerElement()
{
	Super::TriggerElement();

	
}

FVector ARailElement::GetLocationAlongRailSpline(float percent)
{
	if (SplineRail == nullptr)
		return FVector::ZeroVector;

	float PercentToDistance = SplineRail->GetSplineLength() * percent;
	

	FVector OffsetUp = SplineRail->GetTransformAtDistanceAlongSpline(PercentToDistance, ESplineCoordinateSpace::World).GetRotation().GetUpVector();

	OffsetUp.Normalize();
	
	OffsetUp *= 30.f;
	
	return SplineRail->GetLocationAtDistanceAlongSpline(PercentToDistance, ESplineCoordinateSpace::World) + OffsetUp;
}

