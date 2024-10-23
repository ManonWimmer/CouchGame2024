// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsManager.h"


// Sets default values
AEventsManager::AEventsManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEventsManager::BeginPlay()
{
	Super::BeginPlay();

	CheckProbabilities();
}

// Called every frame
void AEventsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsGameStarted)
	{
		if (CurrentTime > GameTimeInSec)
		{
			CurrentTime = GameTimeInSec;
			EndGame();
		}
		else
		{
			CheckAndTriggerEvents();
		}
	}
}

void AEventsManager::StartGame()
{
	StartGameTime = GetWorld()->GetTimeSeconds();
	IsGameStarted = true;
}

void AEventsManager::EndGame()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
												 TEXT("END GAME"));
	IsGameStarted = false;
}

float AEventsManager::GetCountdownTime() const
{
	return GameTimeInSec - CurrentTime;
}

void AEventsManager::CheckAndTriggerEvents()
{
	for (FLevelEventEntry& EventEntry : LevelEvents)
	{
		if (!EventEntry.HasBeenTriggered && CurrentTime > EventEntry.TriggerTime)
		{
			TArray<FEventInfos> EventsInfos = EventEntry.EventArray.Events;

			if (EventsInfos.Num() == 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
				                                 TEXT("NO EVENT INFOS"));
				return;
			}

			float Random = FMath::FRandRange(0.f, 1.f);

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta,
			                                 FString::Printf(TEXT("Random : %f"), Random));

			float LastTotalProbabilities = 0.f;

			for (FEventInfos EventInfo : EventsInfos)
			{
				float CurrentProbability = EventInfo.Probability + LastTotalProbabilities;

				if (CurrentProbability > 1.f)
				{
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
					                                 TEXT("PROBABILITY TOTAL > 1"));
					break;
				}

				if (CurrentProbability > Random)
				{
					EventInfo.Event->TriggerEvent();
					break;
				}

				LastTotalProbabilities += EventInfo.Probability;
			}

			EventEntry.HasBeenTriggered = true;
		}
	}
}

void AEventsManager::CheckProbabilities()
{
	for (FLevelEventEntry& EventEntry : LevelEvents)
	{
		TArray<FEventInfos> EventsInfos = EventEntry.EventArray.Events;

		if (EventsInfos.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			                                 TEXT("NO EVENT INFOS"));
			return;
		}

		float LastTotalProbabilities = 0.f;

		for (FEventInfos EventInfo : EventsInfos)
		{
			float CurrentProbability = EventInfo.Probability + LastTotalProbabilities;

			if (CurrentProbability > 1.f)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
				                                 FString::Printf(
					                                 TEXT("PROBABILITY TOTAL > 1 FOR EVENT : %s"),
					                                 *EventInfo.Event->EventName.ToString()));
				break;
			}

			LastTotalProbabilities += EventInfo.Probability;
		}
	}
}
