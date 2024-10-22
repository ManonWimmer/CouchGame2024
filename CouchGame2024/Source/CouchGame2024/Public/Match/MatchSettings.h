// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MatchSettings.generated.h"

class APlayerBall;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName = "Match Settings"))
class COUCHGAME2024_API UMatchSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category="PlayersBall")
	TSubclassOf<APlayerBall> PlayerBallClassP0;

	UPROPERTY(Config, EditAnywhere, Category="PlayersBall")
	TSubclassOf<APlayerBall> PlayerBallClassP1;

	UPROPERTY(Config, EditAnywhere, Category="PlayersBall")
	TSubclassOf<APlayerBall> PlayerBallClassP2;

	UPROPERTY(Config, EditAnywhere, Category="PlayersBall")
	TSubclassOf<APlayerBall> PlayerBallClassP3;
};
