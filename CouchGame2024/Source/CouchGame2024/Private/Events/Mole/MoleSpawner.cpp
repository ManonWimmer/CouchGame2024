// Fill out your copyright notice in the Description page of Project Settings.


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

	Bind();
}

// Called every frame
void AMoleSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCanSpawn && bInGame)
	{
		CurrentTime = GetWorld()->GetTimeSeconds() - StartedTime;

		if (CurrentTime > RandomSpawnTime)
		{
			//if (GEngine)
			//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s : Current Time > Random"), *this->GetName()));
			
			SpawnMole();
		}
	}
	else if (bCanStay && bInGame)
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
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Start mole countdown spawning");

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
	if (SpawnedMole)
	{
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("%s : Spawned Mole : %s"), *this->GetName(), *SpawnedMole->GetName()));

		SpawnedMole->Destroy();
		SpawnedMole = nullptr;
	}
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "Spawn Duck");

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%s : Pass test"), *this->GetName()));

	// Spawn Mole

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnedMole = GetWorld()->SpawnActor<AMoleElement>(MoleClass, GetActorLocation(), GetActorRotation(), SpawnParameters);

	if (SpawnedMole == nullptr)
	{
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("%s : Return because of nullptr"), *this->GetName()));
	
		return;
	}

	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%s : Spawn"), *this->GetName()));
	
	SpawnedMole->OnMoleCollected.AddDynamic(this, &AMoleSpawner::StartCountdownSpawning);

	BindDespawnMoleToEffect();

	StopCountdownSpawning();
	StartCountdownStaying();

	ReceiveOnSpawnMole();
}

void AMoleSpawner::DestroyMole()
{
	if (SpawnedMole)
	{
		SpawnedMole->DespawnMole();
		UnbindDespawnMoleToEffect();
		SpawnedMole = nullptr;
		StartCountdownSpawning();
	}
}

void AMoleSpawner::BindDespawnMoleToEffect()
{
	if (SpawnedMole == nullptr)	return;

	SpawnedMole->OnMoleDespawn.AddDynamic(this, &AMoleSpawner::ReceiveOnDespawnMole);
}

void AMoleSpawner::UnbindDespawnMoleToEffect()
{
	if (SpawnedMole == nullptr)	return;

	SpawnedMole->OnMoleDespawn.RemoveDynamic(this, &AMoleSpawner::ReceiveOnDespawnMole);
}

void AMoleSpawner::GetRandomSpawnTime()
{
	RandomSpawnTime = FMath::RandRange(MinMoleSpawnTime, MaxMoleSpawnTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,FString::Printf(TEXT("Spawn time mole : %f"), RandomSpawnTime));
}

void AMoleSpawner::GetRandomStayTime()
{
	RandomStayTime = FMath::RandRange(MinMoleStayTime, MaxMoleStayTime);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,FString::Printf(TEXT("Stay time mole : %f"), RandomStayTime));
}

void AMoleSpawner::StartSpawning()
{
	bInGame = true;

	StartCountdownSpawning();
}

void AMoleSpawner::StopSpawning()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "stop spawning mole");
	
	bCanSpawn = false;
	bCanStay = false;
	bInGame = false;
	
	if (SpawnedMole)
	{
		UnbindDespawnMoleToEffect();
		ReceiveOnMoleForcedDestroy();
		SpawnedMole->Destroy();
		SpawnedMole = nullptr;
	}
}

void AMoleSpawner::Bind()
{
	if (bHasBeenBind) return;
	
	if (AEventMole* EventMole = Cast<AEventMole>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventMole::StaticClass())))
	{
		bHasBeenBind = true;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Magenta, "BIND MOLE");
		EventMole->OnMoleStartedEvent.AddDynamic(this, &AMoleSpawner::StartSpawning);
		EventMole->OnMoleEndedEvent.AddDynamic(this, &AMoleSpawner::StopSpawning);
	}
	//else
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "CANT FIND MOLE EVENT FROM MOLE SPAWNER");

}

