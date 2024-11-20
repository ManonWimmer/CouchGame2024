// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SoundsSettings.generated.h"

class USoundsData;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Sounds Settings"))
class COUCHGAME2024_API USoundsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UPROPERTY(Config, EditAnywhere, Category="Settings")
	TSoftObjectPtr<USoundsData> SoundsData;
};
