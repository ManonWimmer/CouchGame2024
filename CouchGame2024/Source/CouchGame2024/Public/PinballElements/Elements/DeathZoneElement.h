// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PinballElements/PinballElement.h"
#include "DeathZoneElement.generated.h"

UCLASS()
class COUCHGAME2024_API ADeathZoneElement : public APinballElement
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADeathZoneElement();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
public:
	
	virtual EPinballElementID GetElementID() override;

	virtual void TriggerElement() override;
};
