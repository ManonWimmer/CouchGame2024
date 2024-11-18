// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerPowerUpData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPlayerPowerUpData : public UDataAsset
{
	GENERATED_BODY()


public:

#pragma region Freeze
	
	UPROPERTY(EditAnywhere, Category = "Freeze Power Up")
	float FreezeRange = 40.f;

	UPROPERTY(EditAnywhere, Category = "Freeze Power Up")
	float FreezeDuration = 3.f;

#pragma endregion 

	
};
