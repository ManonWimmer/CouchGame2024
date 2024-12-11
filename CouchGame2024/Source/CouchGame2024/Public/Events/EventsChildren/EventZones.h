// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/Event.h"
#include "EventZones.generated.h"

class AEventZonesV2Manager;
class APillarZone;

UCLASS()
class COUCHGAME2024_API AEventZones : public AEvent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventZones();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
#pragma region Event

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZonesStartedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZonesPhase1StartedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZonesPhase2StartedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FZonesEndedEvent);

	UPROPERTY(BlueprintAssignable)
	FZonesStartedEvent OnZonesStartedEvent;

	UPROPERTY(BlueprintAssignable)
	FZonesPhase1StartedEvent OnZonesPhase1StartedEvent;

	UPROPERTY(BlueprintAssignable)
	FZonesPhase2StartedEvent OnZonesPhase2StartedEvent;

	UPROPERTY(BlueprintAssignable)
	FZonesEndedEvent OnZonesEndedEvent;
	
	virtual void SetupEventPhase1() override;

	virtual void TriggerEventPhase1() override;

	virtual void TriggerEventPhase2() override;
	
	virtual void TickPhase1() override;

	virtual void TickPhase2() override;
	
	virtual void EndEvent() override;

	void OnZonesEventStarted();
	
	UPROPERTY()
	TArray<TObjectPtr<APillarZone>> PillarZones;

	UPROPERTY()
	TObjectPtr<AEventZonesV2Manager> Phase2Manager = nullptr;

	
#pragma endregion
};
