﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event.h"
#include "GameFramework/Actor.h"
#include "Rounds/RoundsPhasesID.h"
#include "Rounds/RoundsResetable.h"
#include "EventsManager.generated.h"

enum class EEventName : uint8;
class UEventData;

USTRUCT(Blueprintable, BlueprintType)
struct FEventInfos
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	AEvent* Event;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (ClampMin = 0.f, ClampMax = 1.f, UIMin = 0.f, UIMax = 1.f))
	float Probability;

	FEventInfos() : Event(nullptr), Probability(1.0f) {}
};


USTRUCT(Blueprintable, BlueprintType)
struct FEventArray
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	TArray<FEventInfos> Events;
};

USTRUCT(BlueprintType)
struct FLevelEventEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event", meta = (ClampMin = 0.f, ClampMax = 100.f, UIMin = 0.f, UIMax = 100.f))
	float TriggerTime; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FEventArray EventArray;

	UPROPERTY()
	bool HasBeenTriggered = false;

	FLevelEventEntry() : TriggerTime(0.0f) {}  
};

UCLASS(Blueprintable)
class COUCHGAME2024_API AEventsManager : public AActor, public IRoundsResetable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventsManager();

	/*
	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category="Events")
	TArray<FLevelEventEntry> LevelEvents;
	*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Events")
	TArray<UEventData*> Events; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void BindCountdownToRoundsPhase();
	
	UFUNCTION()
	void BindCountdownToRoundsChange();

	UFUNCTION()
	void SetupNewRoundEvent(int RoundIndex);
	
	UFUNCTION()
	void CheckStartCountdown(ERoundsPhaseID InRoundsPhaseID);
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Events")
	void StartGame();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Events")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category = "Events")
	float GetCountdownTime() const;
	
	UPROPERTY()
	int GameTimeInSec = 100;

	UPROPERTY()
	int Phase1Time = 100;

	UPROPERTY()
	int Phase2Time = 100;
	
	UPROPERTY()
	float StartGameTime = 0.f;
	
	UPROPERTY()
	float CurrentTime = 0.f;

	UPROPERTY()
	UEventData* LastEventData = nullptr;

	UPROPERTY()
	UEventData* CurrentEventData = nullptr;

	UFUNCTION()
	void RegisterEvent(UEventData* EventData, AEvent* Event);

	UFUNCTION()
	void TriggerEventPhase1(const UEventData* EventData);

	UFUNCTION()
	void TriggerEventPhase2(const UEventData* EventData);

	UFUNCTION()
	void StartEvent(); // show tag
	
	UFUNCTION()
	void EndEvent(); // hide tag

	UFUNCTION()
	UEventData* GetEventDataFromName(EEventName EventName);

	UPROPERTY()
	TMap<UEventData*, AEvent*> EventsMap;
	
private:
	void CheckAndTriggerEvents();

	bool IsGameStarted = false;

	void GetRandomEvent();

	void SetupEventTimes();

	void CreateEvents();


#pragma region Resetable

public:
	virtual void InitResetable() override;
	
	virtual bool IsResetable() override;

	virtual void ResetObject() override;


#pragma endregion 
};
