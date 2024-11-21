// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event.h"
#include "GameFramework/Actor.h"
#include "Rounds/RoundsPhasesID.h"
#include "Rounds/RoundsResetable.h"
#include "EventsManager.generated.h"

class UDuckData;
class AUIManager;
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
	void StartEvent(int RoundIndex);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "Events")
	void ShowWidgetEvent();
	
	UFUNCTION()
	UEventData* GetEventDataFromName(EEventName EventName);

	UFUNCTION()
	AEvent* GetEventClassFromEventData(const UEventData* EventData);

	UPROPERTY()
	TMap<UEventData*, AEvent*> EventsMap;

	UPROPERTY()
	bool bPhase2Triggered = false;

	UPROPERTY()
	TArray<FName> EventsTags;
	
private:
	UFUNCTION()
	void CheckAndTriggerEvents();

	UPROPERTY()
	bool IsGameStarted = false;

	UFUNCTION()
	void GetRandomEvent();

	UFUNCTION()
	void SetupEventTimes();

	UFUNCTION()
	void CreateEvents();

	UFUNCTION()
	void GetTags();

	UFUNCTION()
	void ShowObjectsWithCurrentEventTag();

	UFUNCTION()
	void HideOtherTags();

	UFUNCTION()
	void HideObjectsWithTag(FName TagName) const;
	
	UFUNCTION()
	TArray<AActor*> GetActorsWithTag(FName TagName) const;

	UPROPERTY()
	TObjectPtr<AUIManager> UIManager = nullptr;

#pragma region Resetable

public:
	virtual void InitResetable() override;
	
	virtual bool IsResetable() override;

	virtual void ResetObject() override;

#pragma endregion 
};
