// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/GlobalScoreSubsystem.h"

#pragma region Score

int UGlobalScoreSubsystem::GetScore(int PlayerIndex) const
{
	switch (PlayerIndex)
	{
	case 0:
		return Player0Score;
	case 1:
		return Player1Score;
	case 2:
		return Player2Score;
	case 3:
		return Player3Score;
	default:
		return -1;
	}
}

void UGlobalScoreSubsystem::AddScore(int PlayerIndex, int Value)
{
	switch (PlayerIndex)
	{
		case 0:
			Player0Score += Value;
			break;
		case 1:
			Player1Score += Value;
			break;
		case 2:
			Player2Score += Value;
			break;
		case 3:
			Player3Score += Value;
			break;
		default:
			break;
	}

	ScoreChangedEvent.Broadcast(PlayerIndex, GetScore(PlayerIndex));
}

#pragma endregion Score

#pragma region Duck Counter

int UGlobalScoreSubsystem::GetDuckCounter(int PlayerIndex) const
{
	switch (PlayerIndex)
	{
	case 0:
		return Player0DuckCounter;
	case 1:
		return Player1DuckCounter;
	case 2:
		return Player2DuckCounter;
	case 3:
		return Player3DuckCounter;
	default:
		return -1;
	}
}


void UGlobalScoreSubsystem::AddDuck(int PlayerIndex, int Value)
{
	switch (PlayerIndex)
	{
	case 0:
		Player0DuckCounter += Value;
		break;
	case 1:
		Player1DuckCounter += Value;
		break;
	case 2:
		Player2DuckCounter += Value;
		break;
	case 3:
		Player3DuckCounter += Value;
		break;
	default:
		break;
	}

	DuckCounterChangedEvent.Broadcast(PlayerIndex, GetDuckCounter(PlayerIndex));
}

#pragma endregion Duck Counter

void UGlobalScoreSubsystem::ResetAllScores()
{
	// Score
	Player0Score = 0;
	Player1Score = 0;
	Player2Score = 0;
	Player3Score = 0;

	ScoreChangedEvent.Broadcast(0, GetScore(0)); 
	ScoreChangedEvent.Broadcast(1, GetScore(1)); 
	ScoreChangedEvent.Broadcast(2, GetScore(2)); 
	ScoreChangedEvent.Broadcast(3, GetScore(3));

	// Duck
	Player0DuckCounter = 0;
	Player1DuckCounter = 0;
	Player2DuckCounter = 0;
	Player3DuckCounter = 0;
	
	DuckCounterChangedEvent.Broadcast(0, GetDuckCounter(0));
	DuckCounterChangedEvent.Broadcast(1, GetDuckCounter(1));
	DuckCounterChangedEvent.Broadcast(2, GetDuckCounter(2));
	DuckCounterChangedEvent.Broadcast(3, GetDuckCounter(3));
}

int UGlobalScoreSubsystem::GetIndexPlayerBestScore()
{
	int PlayerIndexBestScore = 0;
	int BestScore = Player0Score;

	if (Player1Score > BestScore)
	{
		PlayerIndexBestScore = 1;
		BestScore = Player1Score;	
	}

	if (Player2Score > BestScore)
	{
		PlayerIndexBestScore = 2;
		BestScore = Player2Score;	
	}

	if (Player3Score > BestScore)
	{
		PlayerIndexBestScore = 3;
		BestScore = Player3Score;	
	}

	return PlayerIndexBestScore;
}
