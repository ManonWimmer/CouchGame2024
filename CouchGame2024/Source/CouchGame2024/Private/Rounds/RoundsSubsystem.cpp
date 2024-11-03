// Fill out your copyright notice in the Description page of Project Settings.


#include "Rounds/RoundsSubsystem.h"

#include "Rounds/RoundsSettings.h"
#include "Rounds/Datas/RoundsData.h"


void URoundsSubsystem::ChangeRound(int NewRoundIndex)
{
	CurrentRoundIndex = NewRoundIndex;
	OnChangeRound.Broadcast(CurrentRoundIndex);
}

void URoundsSubsystem::ChangeToNextRound()
{
	ChangeRound(CurrentRoundIndex + 1);
}

int URoundsSubsystem::GetCurrentRoundIndex()
{
	return CurrentRoundIndex;
}

ERoundsPhaseID URoundsSubsystem::GetCurrentRoundPhaseID()
{
	return CurrentRoundPhaseID;
}

void URoundsSubsystem::ChangeRoundPhase(ERoundsPhaseID RoundsPhaseID)
{
	CurrentRoundPhaseID = RoundsPhaseID;
	OnChangeRoundPhases.Broadcast(CurrentRoundPhaseID);
}

void URoundsSubsystem::StartRoundPhase()
{
	ChangeRoundPhase(PRE_ROUND);
}

void URoundsSubsystem::ChangeToNextRoundPhase()
{
	switch (CurrentRoundPhaseID)
	{
		case NONE:
			break;
		case PRE_ROUND:
			ChangeRoundPhase(IN_ROUND);
			break;
		case IN_ROUND:
			ChangeRoundPhase(POST_ROUND);
			break;
		case POST_ROUND:
			break;

		default:
			break;
	}
}

void URoundsSubsystem::InitRoundsWonByPlayers(int PlayerCount)
{
	for (int i = 0; i < PlayerCount; i++)
	{
		RoundsWonByPlayersIndex.Add(i, 0);
	}
}

int URoundsSubsystem::GetRoundsWonByPlayerIndex(int PlayerIndex)
{
	if (RoundsWonByPlayersIndex.Num() >= PlayerIndex)
		return	-1;
	
	return RoundsWonByPlayersIndex[PlayerIndex];
}

void URoundsSubsystem::IncreaseRoundsWonByPlayerIndex(int PlayerIndex, int IncreaseNumber)
{
	if (RoundsWonByPlayersIndex.Num() >= PlayerIndex)
		return;

	RoundsWonByPlayersIndex[PlayerIndex] += IncreaseNumber;

	CheckForWinPlayer(PlayerIndex);
}

void URoundsSubsystem::CheckForWinPlayer(int PlayerIndex)
{
	if (RoundsWonByPlayersIndex.Num() >= PlayerIndex)
		return;

	const URoundsSettings* RoundsSettings = GetDefault<URoundsSettings>();

	if (RoundsSettings == nullptr)	return;

	if (RoundsSettings->RoundsData == nullptr)	return;
	
	if (RoundsWonByPlayersIndex[PlayerIndex] >= RoundsSettings->RoundsData->RoundsToWin)
	{
		// Handle Win of playerIndex
	}
}
