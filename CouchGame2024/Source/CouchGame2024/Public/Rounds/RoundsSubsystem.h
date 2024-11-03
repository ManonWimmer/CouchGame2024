// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoundsPhasesID.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RoundsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API URoundsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

#pragma region RoundsChange
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeRound, int, NewRoundIndex);

	UPROPERTY()
	FOnChangeRound OnChangeRound;
	
	void ChangeRound(int NewRoundIndex);

	UFUNCTION(BlueprintCallable)
	void ChangeToNextRound();
	
	UFUNCTION(BlueprintCallable)
	int GetCurrentRoundIndex();

#pragma endregion

#pragma region RoundsPhases
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeRoundPhases, ERoundsPhaseID, NewPhaseID);

	UPROPERTY()
	FOnChangeRoundPhases OnChangeRoundPhases;
	
	UFUNCTION(BlueprintCallable)
	ERoundsPhaseID GetCurrentRoundPhaseID();

	void ChangeRoundPhase(ERoundsPhaseID RoundsPhaseID);

	UFUNCTION(BlueprintCallable)
	void StartRoundPhase();
	
	UFUNCTION(BlueprintCallable)
	void ChangeToNextRoundPhase();
	
#pragma endregion

#pragma region RoundsWon
public:
	void InitRoundsWonByPlayers(int PlayerCount);
	
	UFUNCTION(BlueprintCallable)
	int GetRoundsWonByPlayerIndex(int PlayerIndex);

	UFUNCTION(BlueprintCallable)
	void IncreaseRoundsWonByPlayerIndex(int PlayerIndex, int IncreaseNumber);

	UFUNCTION(BlueprintCallable)
	void CheckForWinPlayer(int PlayerIndex);
	
#pragma endregion 

	
private:
	int CurrentRoundIndex = 0;

	ERoundsPhaseID CurrentRoundPhaseID;

	TMap<int, int> RoundsWonByPlayersIndex;
};
