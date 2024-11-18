// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PinballElementID.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPinballElementID : uint8
{
	None = 0,
	Bumper,
	Flipper,
	Pillar,
	DeathZone,
	Rail,
	BoostPad,
	Tourniquet
};
