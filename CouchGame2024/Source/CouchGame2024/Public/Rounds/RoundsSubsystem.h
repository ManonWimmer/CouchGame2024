﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoundsPhasesID.h"
#include "PlayerBall/LockableInput.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RoundsSubsystem.generated.h"

class UGlobalScoreSubsystem;
class IRoundsResetable;
class AMatchPinballGameMode;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API URoundsSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
#pragma region Tickable
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override { return TStatId(); };

#pragma endregion 

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
public:
	void InitRoundSubsystem();

	UFUNCTION()
	void StartRound();

	UFUNCTION()
	void EndRoundChecks();

	void InitTimers();

	void InitRounds();

	void InitFirstPlayerSpecial();

	void InitGamemodeAndBindStartGame();

	void InitGlobalScoreAndReset();

private:
	UPROPERTY()
	TObjectPtr<AMatchPinballGameMode> MatchPinballGameMode;

	UPROPERTY()
	TObjectPtr<UGlobalScoreSubsystem> GlobalScoreSubsystem;
	
#pragma region RoundsChange
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeRound, int, NewRoundIndex);

	UPROPERTY(BlueprintAssignable)
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

	UPROPERTY(BlueprintAssignable)
	FOnChangeRoundPhases OnChangeRoundPhases;
	
	void InitRoundsPhase();
	
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

	int PlayerIndexLastRoundWin = -1;
	
#pragma endregion 

#pragma region Handle Timers
	
private:
	void HandlePreRoundTimer(float DeltaTime);
	void HandleStartingRoundTimer(float DeltaTime);
	void HandlePostRoundTimer(float DeltaTime);
	
#pragma endregion 

#pragma region ResetRound
	
public:
	void AddResetableObject(UObject* InResetableObject);
	void RemoveResetableObjects(UObject* InResetableObject);

private:

	UFUNCTION(BlueprintCallable)
	void ResetRound();

	UPROPERTY()
	TArray<UObject*> ResetableObjects;

#pragma endregion


#pragma region LockInputs Players

public:
	void AddLockableInput(UObject* Input);
	void RemoveLockableInput(UObject* Input);

private:
	
	UPROPERTY()
	TArray<UObject*> LockableInputsInWorld;
	
public:
	UFUNCTION()
	void LockAllPlayerButOne(int PlayerIndexUnlock);

	UFUNCTION()
	void LockAllPlayer();
	
	UFUNCTION()
	void UnlockAllPlayer();

#pragma endregion
	
	

private:
	int CurrentRoundIndex = 0;

	ERoundsPhaseID CurrentRoundPhaseID = ERoundsPhaseID::NONE;

	UPROPERTY()
	TMap<int, int> RoundsWonByPlayersIndex;


	float CurrentPreRoundTimer = 0.f;
	float CurrentStartingRoundTimer = 0.f;
	float CurrentPostRoundTimer = 0.f;
};
