// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/Event.h"
#include "EventMole.generated.h"

UCLASS()
class COUCHGAME2024_API AEventMole : public AEvent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventMole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
#pragma region Event

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoleStartedEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMoleEndedEvent);

	UPROPERTY(BlueprintAssignable)
	FMoleStartedEvent OnMoleStartedEvent;

	UPROPERTY(BlueprintAssignable)
	FMoleEndedEvent OnMoleEndedEvent;
	
	virtual void SetupEventPhase1() override;

	virtual void TriggerEventPhase1() override;

	virtual void TriggerEventPhase2() override;
	
	virtual void TickPhase1() override;

	virtual void TickPhase2() override;
	
	virtual void EndEvent() override;

#pragma endregion
};
