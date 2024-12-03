// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CameraData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UCameraData : public UDataAsset
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraSmoothSpeed = 2.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float CameraOffsetUp = 0.f;

	UPROPERTY(editAnywhere, Category = "Camera")
	float CameraPercentTreshold = 0.6;;
	
	
	UPROPERTY(EditAnywhere, Category="Shakes")
	TSubclassOf<UCameraShakeBase> CameraShakePunch;

	UPROPERTY(EditAnywhere, Category="Shakes")
	TSubclassOf<UCameraShakeBase> CameraShakeImpact;

	UPROPERTY(EditAnywhere, Category="Shakes")
	TSubclassOf<UCameraShakeBase> CameraShakeDeathZone;
};
