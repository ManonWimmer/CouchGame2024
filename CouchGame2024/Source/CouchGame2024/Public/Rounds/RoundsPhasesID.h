﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RoundsPhasesID.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum ERoundsPhaseID
{
	NONE = 0,
	PRE_ROUND,
	STARTING_ROUND,
	IN_ROUND,
	POST_ROUND
};
