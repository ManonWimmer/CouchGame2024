// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JuicinessAnimData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API UJuicinessAnimData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category= "Duck Rotate Anim", BlueprintReadOnly)
	float RotateInfiniteDuckRandomDelayMax = 1.5f;

	UPROPERTY(EditAnywhere, Category= "Duck Rotate Anim", BlueprintReadOnly)
	float RotateInfiniteDuckDuration = 3.f;
	
	
	UPROPERTY(EditAnywhere, Category= "Duck Pick Anim", BlueprintReadOnly)
	float PickJumpDuckDuration = 0.3f;

	UPROPERTY(EditAnywhere, Category= "Duck Pick Anim", BlueprintReadOnly)
	float PickDuckRotateDuration = 0.4f;

	UPROPERTY(EditAnywhere, Category= "Duck Pick Anim", BlueprintReadOnly)
	float PickDuckTargetScaleDivider = 5.f;

	
	UPROPERTY(EditAnywhere, Category= "Duck Scale Appear Anim", BlueprintReadOnly)
	float ScaleAppearDuckDuration = 1.25f;


	UPROPERTY(EditAnywhere, Category= "Duck Bobbing Effect Anim", BlueprintReadOnly)
	float DuckBobbingDuration = 1.5f;

	UPROPERTY(EditAnywhere, Category= "Duck Bobbing Effect Anim", BlueprintReadOnly)
	float DuckBobbingOffset = 50.f;
	
	UPROPERTY(EditAnywhere, Category= "Duck Bobbing Effect Anim", BlueprintReadOnly)
	float DuckBobbingRandomDelayMax = 1.5f;
	

	

	UPROPERTY(EditAnywhere, Category= "Mole Anim", BlueprintReadOnly)
	float ShakeMoleIntensity = 20.f;

	UPROPERTY(EditAnywhere, Category= "Mole Anim", BlueprintReadOnly)
	float ShakeMoleDuration = 5.f;

	UPROPERTY(EditAnywhere, Category= "Mole Anim", BlueprintReadOnly)
	float YoyoMoleDuration = 1.5f;

	UPROPERTY(EditAnywhere, Category= "Mole Anim", BlueprintReadOnly)
	float YoyoMoleAmplitude = 5.f;

	UPROPERTY(EditAnywhere, Category= "Power Up Scale Appear Anim", BlueprintReadOnly)
	float PowerUpScaleAppearDuration = 1.25f;
};
