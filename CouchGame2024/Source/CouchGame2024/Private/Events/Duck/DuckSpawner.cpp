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

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "BEGIN SPAWNER");

	BindDuckStartEvent();

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

void ADuckSpawner::StartCountdownSpawning() // duck collected
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Start duck countdown spawning");

	GetRandomSpawnTime();
	bCanSpawn = true;
	StartedTime = GetWorld()->GetTimeSeconds();
	SpawnedDuck = nullptr;
}

void ADuckSpawner::StopCountdownSpawning() // end event & spawn duck
{
	bCanSpawn = false;
}

void ADuckSpawner::SpawnDuck() // start phase 1 duck event 
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Spawn Duck");

	// Spawn Duck
	SpawnedDuck = GetWorld()->SpawnActor<APowerUpDuck>(DuckClass, GetActorLocation(), GetActorRotation());
	bCanSpawn = false;

	if (SpawnedDuck)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "duck");
		SpawnedDuck->OnDuckCollected.AddDynamic(this, &ADuckSpawner::StartCountdownSpawning);
	}
	else
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "no duck");

	StopCountdownSpawning();
}

void ADuckSpawner::GetRandomSpawnTime()
{
	RandomSpawnTime = FMath::RandRange(MinDuckSpawnTime, MaxDuckSpawnTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,FString::Printf(TEXT("Spawn time duck : %f"), RandomSpawnTime));
}

void ADuckSpawner::BindDuckStartEvent()
{
	if (!bHasBeenBind)
	{
		
		if (AEventDuck* EventDuck = Cast<AEventDuck>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventDuck::StaticClass())))
		{
			bHasBeenBind = true;
			EventDuck->OnDuckStartedEvent.AddDynamic(this, &ADuckSpawner::SpawnDuck);
			EventDuck->OnDuckEndedEvent.AddDynamic(this, &ADuckSpawner::StopSpawning);
		}
		else
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "CANT FIND DUCK EVENT FROM DUCK SPAWNER");
	}
}

void ADuckSpawner::StopSpawning()
{
	bCanSpawn = false;
	if (SpawnedDuck)
		SpawnedDuck->Destroy();
}
