// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/SpawnerPowerUp.h"


// Sets default values
ASpawnerPowerUp::ASpawnerPowerUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASpawnerPowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpawnerPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASpawnerPowerUp::SpawnSpecificPowerUp(EPowerUpID InPowerUpID)	// Spawn this specific power up at spawner location
{
	if (InPowerUpID == EPowerUpID::None)	return;


	switch (InPowerUpID)
	{
	case EPowerUpID::Freeze:

		break;

	case EPowerUpID::Strength:

		break;

	case EPowerUpID::Slippery:
			
		break;

	default:
		break;
	}
}

void ASpawnerPowerUp::SpawnRandomPowerUp()	// Spawn a random power up
{
	EPowerUpID PowerUpIDToSpawn = ChooseRandomPowerUp();

	SpawnSpecificPowerUp(PowerUpIDToSpawn);
}

EPowerUpID ASpawnerPowerUp::ChooseRandomPowerUp()	// Select a random power up
{
	EPowerUpID OutPowerUpID = EPowerUpID::None;
	
	int RandomIndex = FMath::RandRange(0, 2);

	switch (RandomIndex)
	{
		case 0:
			OutPowerUpID = EPowerUpID::Freeze;
			break;

		case 1:
			OutPowerUpID = EPowerUpID::Strength;
			break;

		case 2:
			OutPowerUpID = EPowerUpID::Slippery;
			break;

		default:
			break;
	}
	
	return OutPowerUpID;
}

