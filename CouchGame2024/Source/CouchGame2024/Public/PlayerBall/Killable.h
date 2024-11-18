﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Killable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UKillable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME2024_API IKillable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:

	virtual void Kill() = 0;

	virtual void Respawn() = 0;

};
