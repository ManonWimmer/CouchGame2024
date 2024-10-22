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
}

// Called every frame
void AEventsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CurrentTime = GetWorld()->GetTimeSeconds();
	CheckAndTriggerEvents();
}

void AEventsManager::CheckAndTriggerEvents()
{
	for (FLevelEventEntry& EventEntry : LevelEvents) // Utiliser une référence ici
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
			float LastEventPercentage = 0.f;

			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Magenta,
			                                 FString::Printf(TEXT("Random : %f"), Random));

			float LastTotalProbabilities = 0.f;

			for (FEventInfos EventInfo : EventsInfos)
			{
				if (EventInfo.Probability + LastTotalProbabilities > Random)
				{
					EventInfo.Event->TriggerEvent();
					break;
				}
				else
				{
					LastEventPercentage = EventInfo.Probability;
				}

				LastTotalProbabilities += EventInfo.Probability;
			}

			EventEntry.HasBeenTriggered = true;
		}
	}
}
