// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/SpawnerPowerUp.h"

#include "PowerUp/PowerUpDataType.h"
#include "PowerUp/SpawnerPowerUpData.h"
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

	SetupData();
	
	StartRespawnCooldown();
}

// Called every frame
void ASpawnerPowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	HandleRespawnCooldown(DeltaTime);
}

void ASpawnerPowerUp::SetupData()
{
	if (SpawnerPowerUpData == nullptr)	return;

	BasicRespawnCooldown = SpawnerPowerUpData->BasicRespawnCooldown;
	
	MinRespawnCooldownToAdd = SpawnerPowerUpData->MinRespawnCooldownToAdd;
	MaxRespawnCooldownToAdd = SpawnerPowerUpData->MaxRespawnCooldownToAdd;
}

void ASpawnerPowerUp::ReceiveSpawnerPowerUpCollected()
{
	UnbindStartSpawnToPickPowerUp(SpawnedPowerUp);
	StartRespawnCooldown();
}

void ASpawnerPowerUp::BindStartSpawnToPickPowerUp(APowerUp* InPowerUp)
{
	if (InPowerUp == nullptr)	return;

	InPowerUp->OnPowerUpCollected.AddDynamic(this, &ASpawnerPowerUp::ReceiveSpawnerPowerUpCollected);
}

void ASpawnerPowerUp::UnbindStartSpawnToPickPowerUp(APowerUp* InPowerUp)
{
	if (InPowerUp == nullptr)	return;

	InPowerUp->OnPowerUpCollected.RemoveDynamic(this, &ASpawnerPowerUp::ReceiveSpawnerPowerUpCollected);
}

void ASpawnerPowerUp::SpawnSpecificPowerUp(EPowerUpID InPowerUpID)	// Spawn this specific power up at spawner location
{
	if (InPowerUpID == EPowerUpID::None)	return;

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

	if (SpawnedPowerUp != nullptr)
	{
		StopRespawnCooldown();
		BindStartSpawnToPickPowerUp(SpawnedPowerUp);
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

void ASpawnerPowerUp::HandleRespawnCooldown(float DeltaTime)
{
	if (!bIsInCooldown)	return;
	
	if (CurrentRespawnCooldown >= BasicRespawnCooldown + RespawnCooldownToAdd)
	{
		if (CheckCanSpawnPowerUp())
		{
			SpawnRandomPowerUp();
		}
	}
	else
	{
		CurrentRespawnCooldown += DeltaTime;
	}
}

void ASpawnerPowerUp::StartRespawnCooldown()
{
	SpawnedPowerUp = nullptr;
	CurrentRespawnCooldown = 0.f;

	SetNewRespawnCooldownToAdd();
	
	bIsInCooldown = true;
}

void ASpawnerPowerUp::StopRespawnCooldown()
{
	bIsInCooldown = false;
}

bool ASpawnerPowerUp::CheckCanSpawnPowerUp()
{
	if (SpawnedPowerUp != nullptr)
		return false;

	if (CurrentRespawnCooldown < BasicRespawnCooldown + RespawnCooldownToAdd)
		return false;
	
	return true;
}

void ASpawnerPowerUp::SetNewRespawnCooldownToAdd()
{
	float Percent = FMath::RandRange(0.f, 1.f);
	RespawnCooldownToAdd = FMath::Lerp(MinRespawnCooldownToAdd, MaxRespawnCooldownToAdd, Percent);
}

