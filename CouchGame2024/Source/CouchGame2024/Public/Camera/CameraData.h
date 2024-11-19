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
};
