// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsManager.h"

#include "Events/EventData.h"
#include "Events/EventsChildren/EventDuck.h"
#include "Events/EventsChildren/EventMole.h"
#include "Events/EventsChildren/EventPush.h"
#include "Events/EventsChildren/EventZones.h"
#include "Rounds/RoundsSubsystem.h"


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

	InitResetable();

	CreateEvents();

	BindCountdownToRoundsPhase();
	BindCountdownToRoundsChange();

	SetupNewRoundEvent(0);
	//CheckProbabilities();
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
			CurrentTime += DeltaTime;
			CheckAndTriggerEvents();
		}
	}
}

#pragma region Countdown
void AEventsManager::BindCountdownToRoundsPhase()
{
	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr) return;

	RoundsSubsystem->OnChangeRoundPhases.AddDynamic(this, &AEventsManager::CheckStartCountdown);
}

void AEventsManager::BindCountdownToRoundsChange()
{
	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr) return;

	RoundsSubsystem->OnChangeRound.AddDynamic(this, &AEventsManager::SetupNewRoundEvent);
}

void AEventsManager::CheckStartCountdown(ERoundsPhaseID InRoundsPhaseID)
{
	switch (InRoundsPhaseID)
	{
	case ERoundsPhaseID::IN_ROUND:
		StartGame();
		break;

	default:
		break;
	}
}

float AEventsManager::GetCountdownTime() const
{
	return GameTimeInSec - CurrentTime;
}
#pragma endregion Countdown

void AEventsManager::SetupNewRoundEvent(int RoundIndex)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "setup new round event");
	
	GetRandomEvent();
	SetupEventTimes();
}

void AEventsManager::StartGame()
{
	StartGameTime = GetWorld()->GetTimeSeconds();
	IsGameStarted = true;
}

void AEventsManager::EndGame()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		                                 TEXT("END GAME"));
	IsGameStarted = false;

	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr) return;

	RoundsSubsystem->ChangeToNextRoundPhase();
}

void AEventsManager::RegisterEvent(UEventData* EventData, AEvent* Event)
{
	if (Event)
	{
		EventsMap.Add(EventData, Event);
	}
}

void AEventsManager::TriggerEventPhase1(const UEventData* EventData)
{
}

void AEventsManager::TriggerEventPhase2(const UEventData* EventData)
{
}

void AEventsManager::StartEvent() // show tag
{
}

void AEventsManager::EndEvent() // hide tag
{
}

UEventData* AEventsManager::GetEventDataFromName(EEventName EventName)
{
	if (Events.Num() == 0) return nullptr;

	for (UEventData* Event : Events)
	{
		if (Event->EventName == EventName) return Event;
	}

	return nullptr;
}

void AEventsManager::CheckAndTriggerEvents()
{
	// todo: check phase1 time & switch phase
	
	/*
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
	*/
}

void AEventsManager::GetRandomEvent()
{
	if (CurrentEventData != nullptr) LastEventData = CurrentEventData;

	TArray<UEventData*> EventDataList;

	if (LastEventData != nullptr) // Round > 0
	{
		UEventData* ExcludedEventData = LastEventData;
		EventDataList = Events.FilterByPredicate([ExcludedEventData](const UEventData* Item)
		{
			return Item != ExcludedEventData;
		});
	}
	else
	{
		EventDataList = Events;
	}

	// Check list
	if (EventDataList.Num() <= 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No valid event to choose from!"));
		return;
	}

	// Get random from filtered list
	UEventData* RandomEventData = EventDataList[FMath::RandRange(0, EventDataList.Num() - 1)];

	if (RandomEventData)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
			                                 FString::Printf(
				                                 TEXT("New Random Event: %hhd"), RandomEventData->EventName));

		CurrentEventData = RandomEventData;
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to pick a random event!"));
	}
}

void AEventsManager::SetupEventTimes()
{
	if (CurrentEventData == nullptr) return;

	GameTimeInSec = CurrentEventData->EventRoundTime;

	Phase1Time = FMath::RoundToInt(FMath::RandRange(CurrentEventData->Phase1MinTime, CurrentEventData->Phase1MaxTime));
	Phase2Time = GameTimeInSec - Phase1Time;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
	                                              FString::Printf(
		                                              TEXT("Time - Game : %i, Phase1: %i, Phase2: %i"), GameTimeInSec,
		                                              Phase1Time, Phase2Time));
}

void AEventsManager::CreateEvents()
{
	// Duck
	AEvent* EventDuck = NewObject<AEventDuck>();
	UEventData* EventDuckData = GetEventDataFromName(EEventName::Duck);
	if (EventDuck && EventDuckData)
	{
		RegisterEvent(EventDuckData, EventDuck);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO DUCK EVENT AND/OR DATA");

	// Zones
	AEvent* EventZones = NewObject<AEventZones>();
	UEventData* EventZonesData = GetEventDataFromName(EEventName::Zones);
	if (EventZones && EventZonesData)
	{
		RegisterEvent(EventZonesData, EventZones);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO ZONES EVENT AND/OR DATA");

	// Mole
	AEvent* EventMole = NewObject<AEventMole>();
	UEventData* EventMoleData = GetEventDataFromName(EEventName::Mole);
	if (EventMole && EventMoleData)
	{
		RegisterEvent(EventMoleData, EventMole);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO MOLE EVENT AND/OR DATA");

	// Push
	AEvent* EventPush = NewObject<AEventPush>();
	UEventData* EventPushData = GetEventDataFromName(EEventName::Push);
	if (EventPush && EventPushData)
	{
		RegisterEvent(EventPushData, EventPush);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO PUSH EVENT AND/OR DATA");
}

#pragma region Reset
void AEventsManager::InitResetable()
{
	if (!GetWorld()) return;

	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr) return;

	RoundsSubsystem->AddResetableObject(this);
}

bool AEventsManager::IsResetable()
{
	return true;
}

void AEventsManager::ResetObject()
{
	CurrentTime = 0.f;

	StartGameTime = GetWorld()->GetTimeSeconds();
	IsGameStarted = false;
}
#pragma endregion Reset
