// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpSlippery.h"


// Sets default values
APowerUpSlippery::APowerUpSlippery()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpSlippery::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpSlippery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpSlippery::GetPowerUpID() const
{
	return EPowerUpID::Slippery;
}

void APowerUpSlippery::TriggerPowerUp(int Value)
{
	Super::TriggerPowerUp(Value);
	
}


