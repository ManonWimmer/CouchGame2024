// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/SpawnerPowerUp.h"

#include "PowerUp/PowerUpDataType.h"
#include "PowerUp/Type/PowerUpFreeze.h"
#include "PowerUp/Type/PowerUpSlippery.h"
#include "PowerUp/Type/PowerUpStrength.h"


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

	if (SpawnedPowerUp == nullptr)	return;
	if (GetWorld() == nullptr)	return;

	if (PowerUpDataType == nullptr)	return;

	
	
	switch (InPowerUpID)
	{
	case EPowerUpID::Freeze:
		if (PowerUpDataType->PowerUpTypeFreeze != nullptr)
		{
			SpawnedPowerUp = GetWorld()->SpawnActor<APowerUp>(PowerUpDataType->PowerUpTypeFreeze, this->GetTransform());
		}
		break;

	case EPowerUpID::Strength:
		if (PowerUpDataType->PowerUpTypeStrength != nullptr)
		{
			SpawnedPowerUp = GetWorld()->SpawnActor<APowerUp>(PowerUpDataType->PowerUpTypeStrength, this->GetTransform());
		}
		break;

	case EPowerUpID::Slippery:
		if (PowerUpDataType->PowerUpTypeSlippery != nullptr)
		{
			SpawnedPowerUp = GetWorld()->SpawnActor<APowerUp>(PowerUpDataType->PowerUpTypeSlippery, this->GetTransform());
		}
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

