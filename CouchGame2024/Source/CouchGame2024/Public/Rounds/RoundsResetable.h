// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RoundsResetable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class URoundsResetable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME2024_API IRoundsResetable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual void InitResetable() = 0;
	
	UFUNCTION()
	virtual  bool IsResetable() = 0;
	
	UFUNCTION()
	virtual void ResetObject() = 0;

	
};
