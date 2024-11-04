// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoundsData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API URoundsData : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "FreeForAll")
	int RoundsToWin = 0;

	UPROPERTY(EditAnywhere, Category = "FreeForAll")
	float PreRoundDuration = 1.f;

	UPROPERTY(EditAnywhere, Category = "FreeForAll")
	float StartingRoundDuration = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "FreeForAll")
	float PostRoundDuration = 0.f;
};
