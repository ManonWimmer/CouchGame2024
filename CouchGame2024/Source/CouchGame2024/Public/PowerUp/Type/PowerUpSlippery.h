// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp/PowerUp.h"
#include "PowerUpSlippery.generated.h"

UCLASS()
class COUCHGAME2024_API APowerUpSlippery : public APowerUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUpSlippery();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual EPowerUpID GetPowerUpID() const;
	
	virtual void TriggerPowerUp(int Value);
};
