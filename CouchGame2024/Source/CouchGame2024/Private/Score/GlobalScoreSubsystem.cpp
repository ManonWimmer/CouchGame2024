// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/GlobalScoreSubsystem.h"

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
		case 1:
			Player1Score += Value;
		case 2:
			Player2Score += Value;
		case 3:
			Player3Score += Value;
		default: ;
	}

	ScoreChangedEvent.Broadcast(PlayerIndex, GetScore(PlayerIndex));
}

void UGlobalScoreSubsystem::RemoveScore(int PlayerIndex, int Value)
{
	switch (PlayerIndex)
	{
		case 0:
			Player0Score -= Value;
			if (Player0Score < 0) Player0Score = 0;
		case 1:
			Player1Score -= Value;
			if (Player1Score < 0) Player1Score = 0;
		case 2:
			Player2Score -= Value;
			if (Player2Score < 0) Player2Score = 0;
		case 3:
			Player3Score -= Value;
			if (Player3Score < 0) Player3Score = 0;
		default: ;
	}

	ScoreChangedEvent.Broadcast(PlayerIndex, GetScore(PlayerIndex)); // A voir si on appelle quand meme si on passe de 0 à 0 :)
}

void UGlobalScoreSubsystem::ResetAllScores()
{
	Player0Score = 0;
	Player1Score = 0;
	Player2Score = 0;
	Player3Score = 0;

	ScoreChangedEvent.Broadcast(0, GetScore(0)); // Pareil à voir en fonction de ce que ça fait
	ScoreChangedEvent.Broadcast(1, GetScore(1)); 
	ScoreChangedEvent.Broadcast(2, GetScore(2)); 
	ScoreChangedEvent.Broadcast(3, GetScore(3)); 
}
