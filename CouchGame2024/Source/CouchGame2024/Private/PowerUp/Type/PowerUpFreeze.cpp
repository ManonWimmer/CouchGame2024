// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpFreeze.h"

#include "Match/MatchPinballGameMode.h"


// Sets default values
APowerUpFreeze::APowerUpFreeze()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpFreeze::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APowerUpFreeze::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpFreeze::GetPowerUpID() const
{
	return EPowerUpID::Freeze;
}

void APowerUpFreeze::TriggerPowerUp(int PlayerIndex)
{
	Super::TriggerPowerUp(PlayerIndex);

	
}

