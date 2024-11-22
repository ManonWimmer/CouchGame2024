// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUp/PowerUp.h"
#include "PowerUpDuck.generated.h"

UCLASS()
class COUCHGAME2024_API APowerUpDuck : public APowerUp
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUpDuck();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual EPowerUpID GetPowerUpID() const override;
    
	virtual void TriggerPowerUp(int PlayerIndex) override;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDuckCollected);

	UPROPERTY()
	FDuckCollected OnDuckCollected;

	UFUNCTION()
	void DestroyDuck();

	UFUNCTION()
	void CollectDuck();

	UPROPERTY()
	bool bHasBeenCollected = false;
};
