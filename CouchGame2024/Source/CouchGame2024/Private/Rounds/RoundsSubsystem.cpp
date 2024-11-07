// Fill out your copyright notice in the Description page of Project Settings.


#include "Rounds/RoundsSubsystem.h"

#include "Rounds/RoundsResetable.h"
#include "Rounds/RoundsSettings.h"
#include "Rounds/Datas/RoundsData.h"


void URoundsSubsystem::Tick(float DeltaTime)
{
	switch (CurrentRoundPhaseID)
	{
		case NONE:
			break;
		case PRE_ROUND:
			HandlePreRoundTimer(DeltaTime);
			break;
		case STARTING_ROUND:
			HandleStartingRoundTimer(DeltaTime);
			break;
		case IN_ROUND:
			break;
		case POST_ROUND:
			HandlePostRoundTimer(DeltaTime);
			break;
	}
}

void URoundsSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
}

void URoundsSubsystem::InitRoundSubsystem()
{
	InitRounds();
	
	InitTimers();

	InitRoundsPhase();
}

void URoundsSubsystem::StartRound()
{
	ChangeRoundPhase(ERoundsPhaseID::PRE_ROUND);
}

void URoundsSubsystem::InitTimers()
{
	const URoundsSettings* RoundsSettings = GetDefault<URoundsSettings>();

	if (RoundsSettings == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Warning : Rounds Settings is nullptr");
		return;
	}

	URoundsData* RoundsData = RoundsSettings->RoundsData.LoadSynchronous();
	
	if (RoundsData == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Warning : Rounds Settings Data is nullptr");
		return;
	}

		
	CurrentPreRoundTimer = RoundsData->PreRoundDuration;
	CurrentStartingRoundTimer = RoundsData->StartingRoundDuration;
	CurrentPostRoundTimer = RoundsData->PostRoundDuration;
}

void URoundsSubsystem::InitRounds()
{
	InitRoundsWonByPlayers(4);

	CurrentRoundIndex = 0;
}

void URoundsSubsystem::InitRoundsPhase()
{
	CurrentRoundPhaseID = NONE;
}

void URoundsSubsystem::ChangeRound(int NewRoundIndex)
{
	CurrentRoundIndex = NewRoundIndex;
	OnChangeRound.Broadcast(CurrentRoundIndex);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Change Round");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(CurrentRoundIndex));
	}
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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, "Change Round Phase");
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("New phase Id = %hhd"), CurrentRoundPhaseID));
	}
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
			ChangeRoundPhase(STARTING_ROUND);
			break;
		case STARTING_ROUND:
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
	RoundsWonByPlayersIndex.Empty();
	
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

void URoundsSubsystem::HandlePreRoundTimer(float DeltaTime)
{
	if (CurrentPreRoundTimer > 0.f)
	{
		CurrentPreRoundTimer -= DeltaTime;
	}
	else
	{
		ChangeToNextRoundPhase();
	}
}

void URoundsSubsystem::HandleStartingRoundTimer(float DeltaTime)
{
	if (CurrentStartingRoundTimer > 0.f)
	{
		CurrentStartingRoundTimer -= DeltaTime;
	}
	else
	{
		ChangeToNextRoundPhase();
	}
}

void URoundsSubsystem::HandlePostRoundTimer(float DeltaTime)
{
	if (CurrentPostRoundTimer > 0.f)
	{
		CurrentPostRoundTimer -= DeltaTime;
	}
	else
    {
    	ChangeToNextRoundPhase();
    }
}

void URoundsSubsystem::AddResetableObject(UObject* InResetableObject)
{
	if (InResetableObject == nullptr)	return;
	if (ResetableObjects.Contains(InResetableObject))	return;

	ResetableObjects.Add(InResetableObject);
}

void URoundsSubsystem::RemoveResetableObjects(UObject* InResetableObject)
{
	if (InResetableObject == nullptr)	return;
	if (!ResetableObjects.Contains(InResetableObject))	return;

	ResetableObjects.Remove(InResetableObject);
}

void URoundsSubsystem::ResetRound()
{
	for (UObject* Resetable : ResetableObjects)
	{
		if (Resetable == nullptr)	continue;

		IRoundsResetable* RoundsResetable = Cast<IRoundsResetable>(Resetable);

		if (RoundsResetable == nullptr)	continue;

		if (!RoundsResetable->IsResetable())	continue;

		RoundsResetable->ResetObject();
	}
}
