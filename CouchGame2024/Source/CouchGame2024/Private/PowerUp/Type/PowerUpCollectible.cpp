// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/Type/PowerUpCollectible.h"

#include "Kismet/GameplayStatics.h"
#include "PlayerBall/PlayerBallController.h"
#include "Score/GlobalScoreSubsystem.h"


// Sets default values
APowerUpCollectible::APowerUpCollectible()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APowerUpCollectible::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerUpCollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUpCollectible::GetPowerUpID() const
{
	return EPowerUpID::Collectible;
}

void APowerUpCollectible::TriggerPowerUp(int PlayerIndex)
{
	// Add score
	UGlobalScoreSubsystem* ScoreSubsystem = GetGameInstance()->GetSubsystem<UGlobalScoreSubsystem>();
	if (ScoreSubsystem != nullptr)
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5.f,FColor::Yellow, FString::Printf(TEXT("Add Score ID: %i"), PlayerIndex));
		ScoreSubsystem->AddScore(PlayerIndex, 1);
	}
	
	Super::TriggerPowerUp();	// Destroy object
}

