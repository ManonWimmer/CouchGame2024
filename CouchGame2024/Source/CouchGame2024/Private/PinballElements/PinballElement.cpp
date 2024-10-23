// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/PinballElement.h"


// Sets default values
APinballElement::APinballElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APinballElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APinballElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPinballElementID APinballElement::GetElementID()
{
	return EPinballElementID::None;
}

void APinballElement::TriggerElement()
{
	
}

