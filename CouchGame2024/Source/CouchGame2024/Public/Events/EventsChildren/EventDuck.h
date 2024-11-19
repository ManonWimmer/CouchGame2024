// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Events/Event.h"
#include "EventDuck.generated.h"

UCLASS()
class COUCHGAME2024_API AEventDuck : public AEvent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventDuck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void AddDuckToPlayer(int PlayerIndex);

	UFUNCTION()
	void ResetValues();

	UPROPERTY()
	int Player0DuckCounter = 0;

	UPROPERTY()
	int Player1DuckCounter = 0;
	
	UPROPERTY()
	int Player2DuckCounter = 0;

	UPROPERTY()
	int Player3DuckCounter = 0;

#pragma region Event
	
	virtual void SetupEventPhase1() override;

	virtual void TriggerEventPhase1() override;

	virtual void TriggerEventPhase2() override;
	
	virtual void TickPhase1() override;

	virtual void TickPhase2() override;
	
	virtual void EndEvent() override;

	
#pragma endregion
};
