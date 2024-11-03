﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp/PowerUp.h"
#include "PowerUpStrength.generated.h"

UCLASS()
class COUCHGAME2024_API APowerUpStrength : public APowerUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUpStrength();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual EPowerUpID GetPowerUpID() const override;

	virtual void TriggerPowerUp(int PlayerIndex) override;
};
