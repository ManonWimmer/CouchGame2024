// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/BumperElement.h"


// Sets default values
ABumperElement::ABumperElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABumperElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABumperElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABumperElement::TriggerElement()
{
	Super::TriggerElement();

	PlayBumperGamefeelEffectsBlueprint();
}

EPinballElementID ABumperElement::GetElementID()
{
	return EPinballElementID::Bumper;
}

#pragma region Grappling Interface
FVector ABumperElement::GetHookPosition()
{
	return GetActorLocation();
}

bool ABumperElement::IsHookable()
{
	return bIsHookable;
}

bool ABumperElement::IsPillar()
{
	return false;
}
#pragma endregion

