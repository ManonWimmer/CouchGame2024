// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/Duck/DuckSpawner.h"

#include "Events/EventsManager.h"
#include "Events/EventsChildren/EventDuck.h"
#include "Kismet/GameplayStatics.h"
#include "PowerUp/Type/PowerUpDuck.h"


// Sets default values
ADuckSpawner::ADuckSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADuckSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (AEventDuck* EventDuck = Cast<AEventDuck>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventDuck::StaticClass())))
		EventDuck->OnDuckStartedEvent.AddDynamic(this, &ADuckSpawner::StartCountdownSpawning);
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "CANT FIND DUCK EVENT FROM DUCK SPAWNER");

}

// Called every frame
void ADuckSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanSpawn)
	{
		CurrentTime = GetWorld()->GetTimeSeconds() - StartedTime;

		if (CurrentTime > RandomSpawnTime)
		{
			SpawnDuck();
		}
	}
}

void ADuckSpawner::StartCountdownSpawning() // start phase 1 duck event - duck collected
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Start duck countdown spawning");

	GetRandomSpawnTime();
	bCanSpawn = true;
	StartedTime = GetWorld()->GetTimeSeconds();
}

void ADuckSpawner::StopCountdownSpawning() // end event & spawn duck
{
	bCanSpawn = false;
}

void ADuckSpawner::SpawnDuck()
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Spawn Duck");

	// Spawn Duck
	SpawnedDuck = GetWorld()->SpawnActor<APowerUpDuck>(DuckClass, GetActorLocation(), GetActorRotation());
	SpawnedDuck->OnDuckCollected.AddDynamic(this, &ADuckSpawner::StartCountdownSpawning);

	StopCountdownSpawning();
}

void ADuckSpawner::GetRandomSpawnTime()
{
	RandomSpawnTime = FMath::RandRange(MinDuckSpawnTime, MaxDuckSpawnTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,FString::Printf(TEXT("Spawn time duck : %f"), RandomSpawnTime));
}
