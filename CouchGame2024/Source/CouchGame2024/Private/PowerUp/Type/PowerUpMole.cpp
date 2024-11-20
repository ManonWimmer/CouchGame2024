﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpMole.h"

#include "Score/GlobalScoreSubsystem.h"


class UGlobalScoreSubsystem;
// Sets default values
APowerUpMole::APowerUpMole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpMole::BeginPlay()
{
	Super::BeginPlay();

	SpawnMole();
}

// Called every frame
void APowerUpMole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpMole::GetPowerUpID() const
{
	return EPowerUpID::Mole;
}

void APowerUpMole::TriggerPowerUp(int PlayerIndex)
{
	UGlobalScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	if (ScoreSubsystem != nullptr)
	{
		//ScoreSubsystem->AddDuck(PlayerIndex, 1);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow, FString::Printf(TEXT("Add Duck to player ID: %i"), PlayerIndex));

		OnMoleCollected.Broadcast();
	}

	DespawnMole();
}

void APowerUpMole::SpawnMole()
{
	// timeline go up
}

void APowerUpMole::DespawnMole()
{
	// stop collision
	// timeline go down & only after destroy
	this->Destroy();
}

