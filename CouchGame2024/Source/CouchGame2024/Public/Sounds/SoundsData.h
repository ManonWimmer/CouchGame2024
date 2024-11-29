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
#pragma region Events Sounds

#pragma region DuckSounds
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckCashOutSound;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckPickSound;

	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckStealSound;

	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> GoldenDuckSound;

#pragma endregion

#pragma region MoleSounds

	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	TObjectPtr<UMetaSoundSource> MoleImpactSound;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	TObjectPtr<UMetaSoundSource> MoleInSound;

	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	TObjectPtr<UMetaSoundSource> MoleOutSound;

#pragma endregion 
	
#pragma endregion

	
#pragma region GrapplingSounds

	UPROPERTY(EditAnywhere, Category = "Sounds Grappling")
	TObjectPtr<UMetaSoundSource> GrapplingGrabSound;


#pragma endregion

#pragma region Players

	UPROPERTY(EditAnywhere, Category = "Player Respawn")
	TObjectPtr<UMetaSoundSource> PlayerRespawn;

	UPROPERTY(EditAnywhere, Category = "Player Impact")
	TObjectPtr<UMetaSoundSource> PlayerImpacts;

	UPROPERTY(EditAnywhere, Category = "Player Impact")
	TObjectPtr<UMetaSoundSource> PlayerPunch;

	UPROPERTY(EditAnywhere, Category = "Player Death")
	TObjectPtr<UMetaSoundSource> PlayerDeath;
	
#pragma endregion

#pragma region Power Up

	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	TObjectPtr<UMetaSoundSource> PowerUpFreezeCast;

	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	TObjectPtr<UMetaSoundSource> PowerUpFreezePlayerTouched;

	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	TObjectPtr<UMetaSoundSource> PowerUpFreezeEffectFinished;

	
	UPROPERTY(EditAnywhere, Category = "Power Up Strength")
	TObjectPtr<UMetaSoundSource> PowerUpStrengthCast;

	
	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	TObjectPtr<UMetaSoundSource> PowerUpSlipperyCast;

	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	TObjectPtr<UMetaSoundSource> PowerUpSlipperyDisapear;

	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	TObjectPtr<UMetaSoundSource> PowerUpSlipperyEffect;

	
	
#pragma endregion 

#pragma region Element Reactions
	
	UPROPERTY(EditAnywhere, Category = "Element Bumper")
	TObjectPtr<UMetaSoundSource> BumperImpact;


	UPROPERTY(EditAnywhere, Category = "Element Rail")
	TObjectPtr<UMetaSoundSource> RailEnter;

	UPROPERTY(EditAnywhere, Category = "Element Rail")
	TObjectPtr<UMetaSoundSource> RailExit;
	
#pragma endregion 
	
};
