// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hookable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHookable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME2024_API IHookable
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	virtual FVector GetHookPosition() = 0;

	virtual bool IsHookable() = 0;
public:
};
