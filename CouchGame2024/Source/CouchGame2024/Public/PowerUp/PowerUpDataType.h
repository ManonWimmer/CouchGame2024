// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PowerUpDataType.generated.h"

class APowerUpSlippery;
class APowerUpStrength;
class APowerUpFreeze;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UPowerUpDataType : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpFreeze> PowerUpTypeFreeze;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpStrength> PowerUpTypeStrength;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APowerUpSlippery> PowerUpTypeSlippery;
};
