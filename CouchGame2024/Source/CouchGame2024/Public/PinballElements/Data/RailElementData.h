// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RailElementData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API URailElementData : public UDataAsset
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, Category = "SpawnRailVariant")
	float SpawnProgressRailDuration = 3.f;
	

	UPROPERTY(EditAnywhere, Category = "Rail Expulse")
	float RailExpulseMinForce = 20000.f;
	
	UPROPERTY(EditAnywhere, Category = "Rail Expulse")
	float RailExpulseMaxForce = 30000.f;

	UPROPERTY(EditAnywhere, Category = "Rail Speed")
	float RailSpeedMultiplier = 3.;
	
};
