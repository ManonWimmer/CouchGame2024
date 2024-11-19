// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Event.generated.h"

UCLASS(Blueprintable, BlueprintType)
class COUCHGAME2024_API AEvent : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEvent();

	UPROPERTY()
	FName EventName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupEventPhase1();
	
	virtual void TriggerEventPhase1();

	virtual void TriggerEventPhase2();

	virtual void EndEvent();

	virtual void TickPhase1();

	virtual void TickPhase2();

	UPROPERTY()
	bool bInTickPhase1 = false;

	UPROPERTY()
	bool bInTickPhase2 = false;
};
