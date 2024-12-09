// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsManager.h"

#include "EngineUtils.h"
#include "Events/EventData.h"
#include "Events/EventsChildren/EventDuck.h"
#include "Events/EventsChildren/EventMole.h"
#include "Events/EventsChildren/EventPush.h"
#include "Events/EventsChildren/EventZones.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBall/PlayerBall.h"
#include "PowerUp/SpawnerPowerUp.h"
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

	//RandomEvents.Add(Events[0]); // jamais appelé on s'en fout

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

	// Get Rounds Subsystem
	RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();
	if (RoundsSubsystem == nullptr)
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "MISSING ROUNDS SUBSYSTEM");
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

	//GetRandomEvent();
	//SetupNewRoundEvent(0);
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
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
										 TEXT("START GAME"));
	StartGameTime = GetWorld()->GetTimeSeconds();
	IsGameStarted = true;
	TriggerEventPhase1(CurrentEventData);

	OnEventStartedEvent.Broadcast(CurrentEventData);
}

void AEventsManager::EndGame()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green,
		                                 TEXT("END GAME"));
	IsGameStarted = false;

	if (AEvent* CurrentEvent = GetEventClassFromEventData(CurrentEventData))
		CurrentEvent->EndEvent();

	if (UIManager != nullptr && CurrentEventData != nullptr)
		UIManager->HideWidgetForEvent(CurrentEventData->EventName);
	
	if (RoundsSubsystem == nullptr) return;
	
	RoundsSubsystem->ChangeToNextRoundPhase();

	OnEventEndedEvent.Broadcast();
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
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "START  ");
	UE_LOG(LogTemp, Display, TEXT("Start Event : %hhd, round %i"), CurrentEventData->EventName, RoundIndex);

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
	// ----- ----- OLD RANDOM ----- ----- //
	/*
	if (CurrentEventData != nullptr) LastEventData = CurrentEventData;

	TArray<UEventData*> EventDataList = Events;

	// Exclude last round event from random if there's more than one
	if (EventDataList.Num() > 1)
	{
		if (LastEventData != nullptr) // Round > 0
		{
			GetEventClassFromEventData(LastEventData)->EndEvent();
			UEventData* ExcludedEventData = LastEventData;
			EventDataList = Events.FilterByPredicate([ExcludedEventData](const UEventData* Item)
			{
				return Item != ExcludedEventData;
			});
		}
	}
	
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
	*/
	// ----- ----- OLD RANDOM ----- ----- //

	// ----- ----- NEW RANDOM ----- ----- //
	CurrentRound++;
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Current Round : %i"), (CurrentRound)));
	UE_LOG(LogTemp, Display, TEXT("Current Round : %i"), CurrentRound);
	
	if (RandomEvents.Num() < CurrentRound || RandomEvents.Num() == 0)
	{
		GetFourRandomEvents();
	}
	CurrentEventData = RandomEvents[CurrentRound - 1];
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Current Event : %hhd"), CurrentEventData->EventName));
	UE_LOG(LogTemp, Display, TEXT("Current Event : %hhd"), CurrentEventData->EventName);

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "FOUND CURRENT EVENT - SHOW NEW ROUND");
	//UIManager->ShowNextRound(CurrentEventData);
	UE_LOG(LogTemp, Display, TEXT("FOUND CURRENT EVENT - SHOW NEW ROUND"));
	
	// ----- ----- NEW RANDOM ----- ----- //
}
void AEventsManager::GetFourRandomEvents()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, "Get Four Random Events");
	}

	if (Events.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Events list is empty!"));
		return;
	}

	TArray<TObjectPtr<UEventData>> TempRandomEvents;
	
	if (Events.Num() == 1)
	{
		TempRandomEvents = Events;
	}
	else
	{
		do
		{
			TempRandomEvents = RandomizeList();
		} 
		while (RandomEvents.Num() > 0 && RandomEvents.Last() == TempRandomEvents[0]);
	}
	
	RandomEvents.Append(TempRandomEvents);
	
	for (const auto& TempRandomEvent : RandomEvents)
	{
		UE_LOG(LogTemp, Display, TEXT("Random Event: %"
								"hhd"), TempRandomEvent->EventName);
	}
}

TArray<TObjectPtr<UEventData>> AEventsManager::RandomizeList() const
{
	TArray<TObjectPtr<UEventData>> RandomizedList = Events;

	FRandomStream RandomStream;
	RandomStream.GenerateNewSeed();

	// Shuffle using the FMath::Rand function
	for (int32 i = RandomizedList.Num() - 1; i > 0; --i)
	{
		int32 SwapIndex = RandomStream.RandRange(0, i);
		RandomizedList.Swap(i, SwapIndex);
	}

	return RandomizedList;
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
	const TObjectPtr<AEventDuck> EventDuck = GetWorld()->SpawnActor<AEventDuck>();
	const TObjectPtr<UEventData> EventDuckData = GetEventDataFromName(EEventName::Duck);
	if (EventDuck && EventDuckData)
	{
		RegisterEvent(EventDuckData, EventDuck);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO DUCK EVENT AND/OR DATA");

	// Zones
	const TObjectPtr<AEvent> EventZones = GetWorld()->SpawnActor<AEventZones>();
	const TObjectPtr<UEventData> EventZonesData = GetEventDataFromName(EEventName::Zones);
	if (EventZones && EventZonesData)
	{
		RegisterEvent(EventZonesData, EventZones);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO ZONES EVENT AND/OR DATA");

	// Mole
	const TObjectPtr<AEvent> EventMole = GetWorld()->SpawnActor<AEventMole>();
	const TObjectPtr<UEventData> EventMoleData = GetEventDataFromName(EEventName::Mole);
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "create mole event");
	if (EventMole && EventMoleData)
	{
		RegisterEvent(EventMoleData, EventMole);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO MOLE EVENT AND/OR DATA");

	// Push
	const TObjectPtr<AEvent> EventPush = GetWorld()->SpawnActor<AEventPush>();
	const TObjectPtr<UEventData> EventPushData = GetEventDataFromName(EEventName::Push);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "CREATE EVENT PUSH");
	if (EventPush && EventPushData)
	{
		RegisterEvent(EventPushData, EventPush);

		TArray<TObjectPtr<AActor>> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBall::StaticClass(), Actors);

		for (TObjectPtr<AActor> Actor : Actors)
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Cyan, "SET EVENT PUSH IN PLAYER BALL FROM EVENTS MANAGER");
			const TObjectPtr<APlayerBall> PlayerBall = Cast<APlayerBall>(Actor);
			PlayerBall->EventPush = Cast<AEventPush>(EventPush);
		}

		Cast<AEventPush>(EventPush)->SetEventData(EventPushData);
	}
	else
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "NO PUSH EVENT AND/OR DATA");

	GetTags();

	// Bind Power Up Spawners
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnerPowerUp::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ASpawnerPowerUp* SpawnerPowerUp = Cast<ASpawnerPowerUp>(Actor))
			SpawnerPowerUp->BindToEventsManager();
	}
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
