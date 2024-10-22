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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	float TriggerTime;  // La clé (temps de déclenchement)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Event")
	FEventArray EventArray;

	UPROPERTY()
	bool HasBeenTriggered = false;

	FLevelEventEntry() : TriggerTime(0.0f) {}  // Valeur par défaut
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
};
