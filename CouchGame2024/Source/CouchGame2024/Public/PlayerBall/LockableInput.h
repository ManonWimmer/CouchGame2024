// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LockableInput.generated.h"

// This class does not need to be modified.
UINTERFACE()
class ULockableInput : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME2024_API ILockableInput
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void InitLockableInput() = 0;
	
	virtual bool IsLockableInput() = 0;

	virtual void LockInput() = 0;

	virtual void UnlockInput() = 0;

	virtual int GetLockableInputIndex() = 0;

	virtual bool IsLocked() = 0;
};
