// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "MetasoundSource.h"

#include "SoundsData.generated.h"

/**
 * 
 */
UCLASS()
class COUCHGAME2024_API USoundsData : public UDataAsset
{
	GENERATED_BODY()


public:

#pragma region DuckSounds
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckCashOutSound;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckPickSound;

	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckStealSound;


#pragma endregion

#pragma region GrapplingSounds

	UPROPERTY(EditAnywhere, Category = "Sounds Grappling")
	TObjectPtr<UMetaSoundSource> GrapplingGrabSound;


#pragma endregion 

};
