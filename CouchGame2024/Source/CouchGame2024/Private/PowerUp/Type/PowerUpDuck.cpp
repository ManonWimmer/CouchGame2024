// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpDuck.h"

#include "Score/GlobalScoreSubsystem.h"


// Sets default values
APowerUpDuck::APowerUpDuck()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpDuck::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpDuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpDuck::GetPowerUpID() const
{
	return EPowerUpID::Duck;
}

void APowerUpDuck::TriggerPowerUp(int PlayerIndex)
{
	UGlobalScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	if (ScoreSubsystem != nullptr)
	{
		ScoreSubsystem->AddDuck(PlayerIndex, 1);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow, FString::Printf(TEXT("Add Duck to player ID: %i"), PlayerIndex));
	}
	
	Super::TriggerPowerUp(PlayerIndex); // Destroy object
}

