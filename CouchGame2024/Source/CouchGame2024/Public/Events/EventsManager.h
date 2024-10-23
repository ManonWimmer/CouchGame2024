// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event.h"
#include "GameFramework/Actor.h"
#include "EventsManager.generated.h"

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
class COUCHGAME2024_API AEventsManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventsManager();

	UPROPERTY(config, EditAnywhere, BlueprintReadWrite, Category="Events")
	TArray<FLevelEventEntry> LevelEvents;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float CurrentTime;

	void CheckAndTriggerEvents();

	void CheckProbabilities();
};
