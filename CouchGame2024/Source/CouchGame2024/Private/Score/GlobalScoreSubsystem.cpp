// Fill out your copyright notice in the Description page of Project Settings.


#include "Score/GlobalScoreSubsystem.h"

#include "Events/EventData.h"
#include "Events/EventsManager.h"
#include "Kismet/GameplayStatics.h"
#include "Sounds/SoundSubsystem.h"

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
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Yellow, FString::Printf(TEXT("add score to player %i : %i"), PlayerIndex, Value));
	switch (PlayerIndex)
	{
		case 0:
			Player0Score += Value;
			Player0Score = FMath::Clamp(Player0Score, 0, INT32_MAX);
			break;
		case 1:
			Player1Score += Value;
			Player1Score = FMath::Clamp(Player1Score, 0, INT32_MAX);
			break;
		case 2:
			Player2Score += Value;
			Player2Score = FMath::Clamp(Player2Score, 0, INT32_MAX);	
			break;
		case 3:
			Player3Score += Value;
			Player3Score = FMath::Clamp(Player3Score, 0, INT32_MAX);
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

void UGlobalScoreSubsystem::PlayerInDuckBank(int PlayerIndex, int DuckToPointsMultiplier)
{
	if (bPlayerInBank) return;
	bPlayerInBank = true;
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan, "PlayerInDuckBank");

	int tempCounter = 0;
	
	switch (PlayerIndex)
	{
	case 0:
		tempCounter = Player0DuckCounter;
		Player0DuckCounter = 0;
		AddScore(0, tempCounter * DuckToPointsMultiplier);
		break;
	case 1:
		tempCounter = Player1DuckCounter;
		Player1DuckCounter = 0;
		AddScore(1, tempCounter * DuckToPointsMultiplier);
		break;
	case 2:
		tempCounter = Player2DuckCounter;
		Player2DuckCounter = 0;
		AddScore(2, tempCounter * DuckToPointsMultiplier);
		break;
	case 3:
		tempCounter = Player3DuckCounter;
		Player3DuckCounter = 0;
		AddScore(3, tempCounter * DuckToPointsMultiplier);
		break;
	default:
		break;
	}

	if (tempCounter > 0)
	{
		if (GetWorld() != nullptr)
		{
			USoundSubsystem* Soundsubsystem = GetWorld()->GetSubsystem<USoundSubsystem>();

			if (Soundsubsystem != nullptr)
			{
				Soundsubsystem->PlayDuckCashOutSound();
			}
		}
	}
	
	ScoreChangedEvent.Broadcast(PlayerIndex, GetScore(PlayerIndex));
	DuckCounterChangedEvent.Broadcast(PlayerIndex, GetDuckCounter(PlayerIndex));

	bPlayerInBank = false;
}

void UGlobalScoreSubsystem::StealDuck(int PlayerIndexAdd, int PlayerIndexLose)
{
	// Check if duck event
	if (AEventsManager* EventsManager = Cast<AEventsManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AEventsManager::StaticClass())))
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "event manager");
		if (EventsManager->CurrentEventData != nullptr)
			if (EventsManager->CurrentEventData->EventName != EEventName::Duck) return;
			else
			{
				//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "pas return");
			}
		else return;
	}
	else
	{
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "StealDuck - No events manager");
		return;
	}

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "steal duck");
	
	// Steal duck
	if (GetDuckCounter(PlayerIndexLose) < 1) return;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "StealDuck");

	AddDuck(PlayerIndexAdd, 1);
	AddDuck(PlayerIndexLose, -1);

	if (GetWorld() != nullptr)
	{
		USoundSubsystem* Soundsubsystem = GetWorld()->GetSubsystem<USoundSubsystem>();

		if (Soundsubsystem != nullptr)
		{
			Soundsubsystem->PlayDuckStealSound();
		}
	}
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


