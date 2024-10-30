// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp/PowerUp.h"
#include "PowerUpDash.generated.h"

UCLASS()
class COUCHGAME2024_API APowerUpDash : public APowerUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUpDash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual EPowerUpID GetPowerUpID() const override;

	virtual void TriggerPowerUp() override;
};
