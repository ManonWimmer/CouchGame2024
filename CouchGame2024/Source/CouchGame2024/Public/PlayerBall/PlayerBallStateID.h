// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerBallStateID.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EPlayerBallStateID : uint8
{
	None = 0,
	Idle UMETA(DisplayName="Idle"),
	Move UMETA(DisplayName="Move"),
	Fall UMETA(DisplayName="Fall"),
	Stun UMETA(DisplayName="Stun"),
	Punch UMETA(DisplayName="Punch"),
	Impact UMETA(DisplayName="Impact"),
	Bumped UMETA(DisplayName="Bumped"),
	Grappling UMETA(DisplayName="Grappling"),
	Grappled UMETA(DisplayName="Grappled"),
	Snapping UMETA(DisplayName="Snapping")
};
