﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/Mole/MoleSpawner.h"

#include "Events/EventsChildren/EventMole.h"
#include "Kismet/GameplayStatics.h"
#include "PinballElements/Elements/MoleElement.h"


// Sets default values
AMoleSpawner::AMoleSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMoleSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (AEventMole* EventMole = Cast<AEventMole>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventMole::StaticClass())))
		EventMole->OnMoleStartedEvent.AddDynamic(this, &AMoleSpawner::StartCountdownSpawning);
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "CANT FIND MOLE EVENT FROM DUCK SPAWNER");

}

// Called every frame
void AMoleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanSpawn)
	{
		CurrentTime = GetWorld()->GetTimeSeconds() - StartedTime;

		if (CurrentTime > RandomSpawnTime)
		{
			SpawnMole();
		}
	}
	else if (bCanStay)
	{
		CurrentTime = GetWorld()->GetTimeSeconds() - StartedTime;

		if (CurrentTime > RandomStayTime)
		{
			DestroyMole();
		}
	}
}

void AMoleSpawner::StartCountdownSpawning() // start phase 1 mole event - mole collected
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Start mole countdown spawning");

	GetRandomSpawnTime();
	bCanSpawn = true;
	bCanStay = false;
	StartedTime = GetWorld()->GetTimeSeconds();
}

void AMoleSpawner::StopCountdownSpawning() // end event & spawn mole
{
	bCanSpawn = false;
}

void AMoleSpawner::StartCountdownStaying()
{
	bCanStay = true;
	StartedTime = GetWorld()->GetTimeSeconds();
	GetRandomStayTime();
}

void AMoleSpawner::SpawnMole()
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Spawn Duck");

	// Spawn Mole
	SpawnedMole = GetWorld()->SpawnActor<AMoleElement>(MoleClass, GetActorLocation(), GetActorRotation());
	SpawnedMole->OnMoleCollected.AddDynamic(this, &AMoleSpawner::StartCountdownSpawning);

	StopCountdownSpawning();
	StartCountdownStaying();
}

void AMoleSpawner::DestroyMole()
{
	if (SpawnedMole)
	{
		SpawnedMole->DespawnMole();
		SpawnedMole = nullptr;
		StartCountdownSpawning();
	}
}

void AMoleSpawner::GetRandomSpawnTime()
{
	RandomSpawnTime = FMath::RandRange(MinMoleSpawnTime, MaxMoleSpawnTime);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,FString::Printf(TEXT("Spawn time mole : %f"), RandomSpawnTime));
}

void AMoleSpawner::GetRandomStayTime()
{
	RandomStayTime = FMath::RandRange(MinMoleStayTime, MaxMoleStayTime);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,FString::Printf(TEXT("Stay time mole : %f"), RandomStayTime));
}

