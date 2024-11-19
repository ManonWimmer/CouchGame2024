// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/BoostPadElement.h"


// Sets default values
ABoostPadElement::ABoostPadElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABoostPadElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABoostPadElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPinballElementID ABoostPadElement::GetElementID()
{
	return EPinballElementID::BoostPad;
}

void ABoostPadElement::TriggerElement()
{
	Super::TriggerElement();
	
}

