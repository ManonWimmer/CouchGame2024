// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "SkinsSettings.generated.h"

class USkinsData;
/**
 * 
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Skins Settings"))
class COUCHGAME2024_API USkinsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UPROPERTY(Config, EditAnywhere, Category="Skins Settings")
	TSoftObjectPtr<USkinsData> SkinsDataP1;

	UPROPERTY(Config, EditAnywhere, Category="Skins Settings")
	TSoftObjectPtr<USkinsData> SkinsDataP2;
	
	UPROPERTY(Config, EditAnywhere, Category="Skins Settings")
	TSoftObjectPtr<USkinsData> SkinsDataP3;
    	
	UPROPERTY(Config, EditAnywhere, Category="Skins Settings")
	TSoftObjectPtr<USkinsData> SkinsDataP4;
};
