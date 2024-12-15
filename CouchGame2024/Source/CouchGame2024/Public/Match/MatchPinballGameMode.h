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
	void InitSoundSubsystem();
	void InitSkinsSubsystem();

	virtual void BeginPlay() override;

#pragma region Players

public:
	void SetNewLocationStartPlayerBallsSpecial(int PlayerSpecial);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerBallSpawned);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerBallSpawned OnPlayerBallSpawned;

	UPROPERTY()
	TMap<int, TObjectPtr<APlayerBall>> PlayerBallsMap;
	
private:

	void FindPlayerBallSpawnInWorld(TArray<APlayerBallSpawn*> &ResultsActors);

	void SpawnPlayerBalls(const TArray<APlayerBallSpawn*> SpawnPoints);

	void SetLocationStartPlayerBallsSpecial(const TArray<APlayerBallSpawn*> SpawnPoints, int PlayerSpecial);

	
	TSubclassOf<APlayerBall> GetPlayerBallClassFromInputType(EAutoReceiveInput::Type InputType) const;
	
	int GetPlayerIndexFromInputType(EAutoReceiveInput::Type InputType) const;

	void CreateAndInitPlayers() const;

	bool CheckPlayerBallIsConnected(int InPlayerIndex) const;

	void DisableNewPlayerConnectedSpawn();
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<APlayerBall*> PlayersBallInsideArena;

	UPROPERTY()
	TArray<APlayerBallSpawn*> PlayerBallSpawns;
	
#pragma endregion

#pragma region Game Match

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBeginGameMatch);

	UPROPERTY()
	FOnBeginGameMatch OnBeginGameMatch;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEndGameMatch);

	UPROPERTY()
	FOnEndGameMatch OnEndGameMatch;
	
	void BeginGame();

	void EndGame();

#pragma endregion 
	
#pragma region Rounds

private:
	UFUNCTION()
	void InitRoundsSubsystem();

#pragma endregion

#pragma region Win

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMatchWin, int, PlayerWinIndex);

	UPROPERTY(BlueprintAssignable)
	FOnMatchWin OnMatchWin;

	UFUNCTION()
	void MatchWin(int InWinPlayerIndex);
	
private:
	UPROPERTY()
	int WinPlayerIndex = -1;
	

#pragma endregion 
};
