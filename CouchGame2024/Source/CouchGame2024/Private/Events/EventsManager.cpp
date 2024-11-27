﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsManager.h"

#include "EngineUtils.h"
#include "Events/EventData.h"
#include "Events/EventsChildren/EventDuck.h"
#include "Events/EventsChildren/EventMole.h"
#include "Events/EventsChildren/EventPush.h"
#include "Events/EventsChildren/EventZones.h"
#include "Kismet/GameplayStatics.h"
#include "Rounds/RoundsSubsystem.h"
#include "UI/UIManager.h"
#include "Zone/EventZonesV2Manager.h"


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

	// Get UI Manager
	UIManager = Cast<AUIManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AUIManager::StaticClass()));
	if (UIManager == nullptr)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "MISSING UI MANAGER");
	}
	
	InitResetable();

	//FTimerHandle TimerHandle;
	//float DelayTime = 1.f;
	//GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AEventsManager::Setup, DelayTime, false); 

	Setup();
	
	// CreateEvents();
	//
	// BindCountdownToRoundsPhase();
	// BindCountdownToRoundsChange();
	//
	// SetupNewRoundEvent(0);
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

void AEventsManager::Setup()
{
	CreateEvents();

	BindCountdownToRoundsPhase();
	BindCountdownToRoundsChange();

	GetRandomEvent();
	SetupNewRoundEvent(0);
}

#pragma region Countdown
void AEventsManager::BindCountdownToRoundsPhase()
{
	RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr) return;

	RoundsSubsystem->OnChangeRoundPhases.AddDynamic(this, &AEventsManager::CheckNewPhase);
}

void AEventsManager::BindCountdownToRoundsChange()
{
	RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr) return;

	RoundsSubsystem->OnChangeRound.AddDynamic(this, &AEventsManager::SetupNewRoundEvent);
}

void AEventsManager::CheckNewPhase(ERoundsPhaseID InRoundsPhaseID)
{
	switch (InRoundsPhaseID)
	{
	case ERoundsPhaseID::IN_ROUND:
		StartGame();
		break;
	case ERoundsPhaseID::POST_ROUND:
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "POST_ROUND");
		if (RoundsSubsystem->bPlayerHasWonGame)
		{
			UIManager->ShowWinner();
		}
		else
		{
			GetRandomEvent();
			UIManager->ShowNextRound(CurrentEventData);
		}
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

	
	SetupEventTimes();
	StartEvent(RoundIndex);
}

void AEventsManager::StartGame()
{
	StartGameTime = GetWorld()->GetTimeSeconds();
	IsGameStarted = true;
	TriggerEventPhase1(CurrentEventData);
}

void AEventsManager::EndGame()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		                                 TEXT("END GAME"));
	IsGameStarted = false;

	if (UIManager != nullptr && CurrentEventData != nullptr)
		UIManager->HideWidgetForEvent(CurrentEventData->EventName);

	RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

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
	if (AEvent* CurrentEvent = GetEventClassFromEventData(EventData))
		CurrentEvent->TriggerEventPhase1();
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                              "COULDN'T FIND CURRENT EVENT TO TRIGGER PHASE 1");
}

void AEventsManager::TriggerEventPhase2(const UEventData* EventData)
{
	if (AEvent* CurrentEvent = GetEventClassFromEventData(EventData))
		CurrentEvent->TriggerEventPhase2();
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                              "COULDN'T FIND CURRENT EVENT TO TRIGGER PHASE 2");
}

void AEventsManager::StartEvent(int RoundIndex)
{
	// Hide or show objects depending on current tag
	ShowObjectsWithCurrentEventTag();
	
	// Setup phase 1 
	if (AEvent* CurrentEvent = GetEventClassFromEventData(CurrentEventData))
		CurrentEvent->SetupEventPhase1();

	// Show UI (pas appeler au round 1 parce que pas créé)
	if (RoundIndex > 0)
		ShowWidgetEvent();
}

void AEventsManager::ShowWidgetEvent()
{
	if (UIManager != nullptr && CurrentEventData != nullptr)
		UIManager->ShowWidgetForEvent(CurrentEventData->EventName);
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

AEvent* AEventsManager::GetEventClassFromEventData(const UEventData* EventData)
{
	if (AEvent** FoundEvent = EventsMap.Find(EventData))
		return *FoundEvent;

	return nullptr;
}

void AEventsManager::CheckAndTriggerEvents()
{
	if (!bPhase2Triggered && CurrentTime > Phase1Time)
	{
		bPhase2Triggered = true;
		TriggerEventPhase2(CurrentEventData);
	}
}

void AEventsManager::GetRandomEvent()
{
	if (CurrentEventData != nullptr) LastEventData = CurrentEventData;

	TArray<UEventData*> EventDataList;


	/*
	if (LastEventData != nullptr) // Round > 0
	{
		GetEventClassFromEventData(LastEventData)->EndEvent();
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
	*/

	EventDataList = Events;
	// Check list
	if (EventDataList.Num() <= 0)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No valid event to choose from!"));
		return;
	}

	// Get random from filtered list
	if (UEventData* RandomEventData = EventDataList[FMath::RandRange(0, EventDataList.Num() - 1)])
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

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,
		                                 FString::Printf(
			                                 TEXT("Time - Game : %i, Phase1: %i, Phase2: %i"), GameTimeInSec,
			                                 Phase1Time, Phase2Time));
}

void AEventsManager::CreateEvents()
{
	// Duck
	AEventDuck* EventDuck = GetWorld()->SpawnActor<AEventDuck>();
	UEventData* EventDuckData = GetEventDataFromName(EEventName::Duck);
	if (EventDuck && EventDuckData)
	{
		RegisterEvent(EventDuckData, EventDuck);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO DUCK EVENT AND/OR DATA");

	// Zones
	AEvent* EventZones = GetWorld()->SpawnActor<AEventZones>();
	UEventData* EventZonesData = GetEventDataFromName(EEventName::Zones);
	if (EventZones && EventZonesData)
	{
		RegisterEvent(EventZonesData, EventZones);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO ZONES EVENT AND/OR DATA");

	// Mole
	AEvent* EventMole = GetWorld()->SpawnActor<AEventMole>();
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "create mole event");
	UEventData* EventMoleData = GetEventDataFromName(EEventName::Mole);
	if (EventMole && EventMoleData)
	{
		RegisterEvent(EventMoleData, EventMole);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO MOLE EVENT AND/OR DATA");

	// Push
	AEvent* EventPush = GetWorld()->SpawnActor<AEventPush>();
	UEventData* EventPushData = GetEventDataFromName(EEventName::Push);
	if (EventPush && EventPushData)
	{
		RegisterEvent(EventPushData, EventPush);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO PUSH EVENT AND/OR DATA");

	GetTags();
}

#pragma region Tags

void AEventsManager::GetTags()
{
	EventsTags.Reset();
	EventsTags.Add(TEXT("Constant"));

	for (const TTuple<UEventData*, AEvent*> Item : EventsMap)
	{
		EventsTags.Add(Item.Key->EventTag);
		//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("add tag %s"), *Item.Key->EventTag.ToString()));
	}
}

void AEventsManager::ShowObjectsWithCurrentEventTag()
{
	if (CurrentEventData == nullptr) return;
	
	HideOtherTags();
	
	TArray<AActor*> Actors = GetActorsWithTag(CurrentEventData->EventTag);
	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(false);
			Actor->SetActorEnableCollision(true);
			UE_LOG(LogTemp, Log, TEXT("%s set visible to true."), *Actor->GetName());
		}
	}
}

void AEventsManager::HideOtherTags()
{
	for (FName Tag : EventsTags)
	{
		if (Tag != CurrentEventData->EventTag && Tag != "Constant")
		{
			HideObjectsWithTag(Tag);
		}
	}
}

void AEventsManager::HideObjectsWithTag(FName TagName) const
{
	TArray<AActor*> Actors = GetActorsWithTag(TagName);
	for (AActor* Actor : Actors)
	{
		if (Actor)
		{
			Actor->SetActorHiddenInGame(true);
			Actor->SetActorEnableCollision(false);
			UE_LOG(LogTemp, Log, TEXT("%s set visible to false."), *Actor->GetName());
		}
	}
}

TArray<AActor*> AEventsManager::GetActorsWithTag(FName TagName) const
{
	TArray<AActor*> TaggedActors;
	
	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor && Actor->Tags.Contains(TagName))
		{
			TaggedActors.Add(Actor);
		}
	}

	return TaggedActors;
}

#pragma endregion Tags

#pragma region Reset
void AEventsManager::InitResetable()
{
	if (!GetWorld()) return;

	RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

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
	bPhase2Triggered = false;
}
#pragma endregion Reset
