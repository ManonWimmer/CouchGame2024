// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraFollowTarget.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCameraFollowTarget : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COUCHGAME2024_API ICameraFollowTarget
{
	GENERATED_BODY()

	
public:
	virtual void InitFollowTarget() = 0;
	
    virtual FVector GetFollowPosition() const = 0;

    virtual bool IsFollowable() const = 0;
};
