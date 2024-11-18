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
	Idle,
	Move,
	Fall,
	Stun,
	Punch,
	Impact,
	Bumped,
	Grappling,
	Grappled,
	Snapping,
	Locked,
	Death,
	Respawn,
	Rail,
	Tourniquet,
	PowerUpHub,
	Freeze,
	Strength
};
