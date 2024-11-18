// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/TourniquetElement.h"

#include "Components/SphereComponent.h"


// Sets default values
ATourniquetElement::ATourniquetElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	SphereTrigger = CreateDefaultSubobject<USphereComponent>("SphereTrigger");
	
	TourniquetAnchorTurning = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorTurning");
	
	TourniquetAnchorPlayer0 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer0");
	TourniquetAnchorPlayer1 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer1");
	TourniquetAnchorPlayer2 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer2");
	TourniquetAnchorPlayer3 = CreateDefaultSubobject<USceneComponent>("TourniquetAnchorPlayer3");

	RootComponent = SphereTrigger;
	
	TourniquetAnchorTurning->SetupAttachment(SphereTrigger);

	TourniquetAnchorPlayer0->SetupAttachment(TourniquetAnchorTurning);
	TourniquetAnchorPlayer1->SetupAttachment(TourniquetAnchorTurning);
	TourniquetAnchorPlayer2->SetupAttachment(TourniquetAnchorTurning);
	TourniquetAnchorPlayer3->SetupAttachment(TourniquetAnchorTurning);
}

// Called when the game starts or when spawned
void ATourniquetElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATourniquetElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleTurningEffect(DeltaTime);
}

EPinballElementID ATourniquetElement::GetElementID()
{
	return EPinballElementID::Tourniquet;
}

void ATourniquetElement::TriggerElement()
{
	Super::TriggerElement();

	
}

void ATourniquetElement::HandleTurningEffect(float DeltaTime)
{
	if (TourniquetAnchorTurning == nullptr)	return;
	
	FRotator RotationToAdd = FRotator(0, TurningSpeed, 0);
	
	TourniquetAnchorTurning->AddLocalRotation(RotationToAdd * DeltaTime);
}

