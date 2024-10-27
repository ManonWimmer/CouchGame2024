// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchPinballGameMode.generated.h"

class APlayerBall;
class APlayerBallSpawn;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API AMatchPinballGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:

	void FindPlayerBallSpawnInWorld(TArray<APlayerBallSpawn*> &ResultsActors);

	void SpawnPlayerBalls(const TArray<APlayerBallSpawn*> SpawnPoints);

	TSubclassOf<APlayerBall> GetPlayerBallClassFromInputType(EAutoReceiveInput::Type InputType) const;

	void CreateAndInitPlayers() const;

protected:
	UPROPERTY(EditAnywhere)
	TArray<APlayerBall*> PlayersBallInsideArena;
};
