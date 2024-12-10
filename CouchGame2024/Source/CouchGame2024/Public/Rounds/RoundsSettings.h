// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RoundsSettings.generated.h"

class URoundsData;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Rounds Settings"))
class COUCHGAME2024_API URoundsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="Settings", BlueprintReadOnly)
	TSoftObjectPtr<URoundsData> RoundsData;
};
