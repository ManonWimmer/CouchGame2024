// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SpawnerPowerUpData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API USpawnerPowerUpData : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, Category = "Basic Cooldown")
	float BasicRespawnCooldown = 5.f;
	
	UPROPERTY(EditAnywhere, Category = "Random Cooldown To add")
	float MinRespawnCooldownToAdd = 2.f;

	UPROPERTY(EditAnywhere, Category = "Random Cooldown To add")
	float MaxRespawnCooldownToAdd = 5.f;
	
};
