// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpHeavy.h"


// Sets default values
APowerUpHeavy::APowerUpHeavy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpHeavy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpHeavy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpHeavy::GetPowerUpID() const
{
	return EPowerUpID::Heavy;
}

void APowerUpHeavy::TriggerPowerUp(int PlayerIndex)
{
	Super::TriggerPowerUp(PlayerIndex);
	
}

