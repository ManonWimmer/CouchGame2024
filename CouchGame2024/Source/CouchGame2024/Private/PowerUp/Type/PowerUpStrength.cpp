// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpStrength.h"


// Sets default values
APowerUpStrength::APowerUpStrength()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpStrength::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpStrength::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpStrength::GetPowerUpID() const
{
	return EPowerUpID::Strength;
}

void APowerUpStrength::TriggerPowerUp(int PlayerIndex)
{
	Super::TriggerPowerUp(PlayerIndex);
	
}

