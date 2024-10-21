﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerBallData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerBallData : public UDataAsset
{
	GENERATED_BODY()

public:

#pragma region Roll Movements
	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float AngularRollForce = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float AngularRollDamping = 2.f;

	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float MaxAngularRollVelocity = 12000.f;

	UPROPERTY(EditAnywhere, Category = "Roll Movements")
	float BraqueDirectionForceMultiplier = 2.f;
#pragma endregion

#pragma region Falling Movements
	UPROPERTY(EditAnywhere, Category = "Falling Movements")
	float GravityScale = 1.f;



#pragma endregion

	
};
