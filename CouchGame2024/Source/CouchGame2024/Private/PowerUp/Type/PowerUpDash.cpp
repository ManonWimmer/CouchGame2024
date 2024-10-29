// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpDash.h"


// Sets default values
APowerUpDash::APowerUpDash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpDash::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpDash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpDash::GetPowerUpID() const
{
	return EPowerUpID::Dash;
}

