// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

class ARailElement;
class APlayerBallSpawn;
class APlayerBall;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	
public:
	virtual void BeginPlay() override;

#pragma region Players

public:
	void SetNewLocationStartPlayerBallsSpecial(int PlayerSpecial);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerBallSpawned);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerBallSpawned OnPlayerBallSpawned;
	
private:

	void FindPlayerBallSpawnInWorld(TArray<APlayerBallSpawn*> &ResultsActors);

	void SpawnPlayerBalls(const TArray<APlayerBallSpawn*> SpawnPoints);

	void SetLocationStartPlayerBallsSpecial(const TArray<APlayerBallSpawn*> SpawnPoints, int PlayerSpecial);

	TSubclassOf<APlayerBall> GetPlayerBallClassFromInputType(EAutoReceiveInput::Type InputType) const;

	int GetPlayerIndexFromInputType(EAutoReceiveInput::Type InputType) const;
	
	void CreateAndInitPlayers() const;

	void ReasignExistingPlayers() const;
	
protected:
	UPROPERTY(EditAnywhere)
	TArray<APlayerBall*> PlayersBallInsideArena;

	UPROPERTY()
	TArray<APlayerBallSpawn*> PlayerBallSpawns;
	
#pragma endregion

#pragma region Multiplayer Spawns

private:
	void BindSpawnPlayerBallToNewPlayerConnected();

	UFUNCTION()
	void SpawnNewPlayerBallByPlayerConnected(int InPlayerIndex);

	UPROPERTY()
	TArray<ARailElement*> ConnectionRailElements;

	void InitConnectionRailElements();

	void DisconnectAllPlayersBall();
public:
	ARailElement* GetConnectionRailElements(int InIndex) const;
	
#pragma endregion 
	
};

