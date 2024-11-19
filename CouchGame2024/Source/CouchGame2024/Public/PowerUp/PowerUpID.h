// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PowerUpID.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPowerUpID : uint8
{
 None = 0,
 Dash,
 Collectible,
 Freeze,
 Strength,
 Slippery,
 Heavy,
 Duck
};