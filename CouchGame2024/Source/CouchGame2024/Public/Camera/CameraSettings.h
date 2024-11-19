// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "CameraSettings.generated.h"

class UCameraData;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Camera Subsystem Settings"))
class COUCHGAME2024_API UCameraSettings : public UDeveloperSettings
{
	GENERATED_BODY()


public:

	UPROPERTY(Config, EditAnywhere, Category="Settings")
	TSoftObjectPtr<UCameraData> CameraData;
};
