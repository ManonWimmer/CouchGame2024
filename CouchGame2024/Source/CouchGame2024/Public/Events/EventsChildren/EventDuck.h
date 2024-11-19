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
};
