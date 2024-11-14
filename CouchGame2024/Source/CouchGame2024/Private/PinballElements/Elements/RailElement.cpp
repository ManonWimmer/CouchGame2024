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

	RootComponent = RootSceneComp;

	SplineRail->SetupAttachment(RootSceneComp);
	SphereTriggerRail->SetupAttachment(SplineRail);
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
	
	return SplineRail->GetLocationAtDistanceAlongSpline(SplineRail->GetSplineLength() * percent, ESplineCoordinateSpace::World);
}

