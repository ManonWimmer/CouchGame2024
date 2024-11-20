// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TourniquetElementData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UTourniquetElementData : public UDataAsset
{
	GENERATED_BODY()


public:

#pragma region Duration
	
	UPROPERTY(EditAnywhere, Category = "Tourniquet Duration")
	float TurningDuration = 3.f;

#pragma endregion

#pragma region Speed
	
	UPROPERTY(EditAnywhere, Category = "Tourniquet Speed")
	float MinTurningTurningSpeed = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Tourniquet Speed")
	float MaxTurningTurningSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Tourniquet Speed")
	float TurningSpeedDecceleration = 60.f;

#pragma endregion 
	
#pragma region Expulse Force
	UPROPERTY(EditAnywhere, Category = "Tourniquet Expulse Force")
	float MinExpulseForce = 20000.f;

	UPROPERTY(EditAnywhere, Category = "Tourniquet Expulse Force")
	float MaxExpulseForce = 60000.f;

#pragma endregion 
	
};
