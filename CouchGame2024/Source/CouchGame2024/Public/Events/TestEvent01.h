// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Event.h"
#include "TestEvent01.generated.h"

UCLASS()
class COUCHGAME2024_API ATestEvent01 : public AEvent
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATestEvent01();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
