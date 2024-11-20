﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SoundSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API USoundSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
private:
	void InitSoundSubsystem();
	
};
