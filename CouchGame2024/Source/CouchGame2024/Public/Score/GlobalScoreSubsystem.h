// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlobalScoreSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UGlobalScoreSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FScoreChangedEvent, int, PlayerIndex, int, NewScore);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDuckCounterChangedEvent, int, PlayerIndex, int, NewScore);

	UFUNCTION(BlueprintCallable)
	int GetScore(int PlayerIndex) const;

	UFUNCTION(BlueprintCallable)
	int GetDuckCounter(int PlayerIndex) const;

	UFUNCTION(BlueprintCallable)
	void AddScore(int PlayerIndex, int Value);

	UFUNCTION(BlueprintCallable)
	void AddDuck(int PlayerIndex, int Value);

	UFUNCTION(BlueprintCallable)
	void ResetAllScores();

	UFUNCTION(BlueprintCallable)
	int GetIndexPlayerBestScore();

	UFUNCTION(BlueprintCallable)
	void PlayerInDuckBank(int PlayerIndex, int DuckToPointsMultiplier);

	UFUNCTION(BlueprintCallable)
	void StealDuck(int PlayerIndexAdd, int PlayerIndexLose);
	
protected:
	UPROPERTY(BlueprintReadOnly)
	int Player0Score = 0;

	UPROPERTY(BlueprintReadOnly)
	int Player1Score = 0;

	UPROPERTY(BlueprintReadOnly)
	int Player2Score = 0;

	UPROPERTY(BlueprintReadOnly)
	int Player3Score = 0;

	UPROPERTY(BlueprintReadOnly)
	int Player0DuckCounter = 0;

	UPROPERTY(BlueprintReadOnly)
	int Player1DuckCounter = 0;
	
	UPROPERTY(BlueprintReadOnly)
	int Player2DuckCounter = 0;

	UPROPERTY(BlueprintReadOnly)
	int Player3DuckCounter = 0;
	
	UPROPERTY(BlueprintAssignable)
	FScoreChangedEvent ScoreChangedEvent;
	
	UPROPERTY(BlueprintAssignable)
	FDuckCounterChangedEvent DuckCounterChangedEvent;
};
