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
	
#pragma region Music Sounds

	UPROPERTY(EditAnywhere, Category = "Sounds Music")
	TObjectPtr<UMetaSoundSource> InGameMusic;
	UPROPERTY(EditAnywhere, Category = "Sounds Music")
	float InGameMusicAdjuster = 1.f;
	
#pragma endregion 

#pragma region Rounds Sounds

	UPROPERTY(EditAnywhere, Category = "Sounds Round")
	TObjectPtr<UMetaSoundSource> RoundStartSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Round")
	float RoundStartSoundAdjuster = 1.f;


	UPROPERTY(EditAnywhere, Category = "Sounds Crowd")
	TObjectPtr<UMetaSoundSource> CrowdEffectSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Crowd")
	float CrowdEffectSoundAdjuster = 1.f;

#pragma endregion 

#pragma region UI Sounds

#pragma region UI Buttons
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> ButtonPressSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float ButtonPressSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> CheckSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float CheckSoundAdjuster = 1.f;

	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> HoverSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float HoverSoundAdjuster = 1.f;

	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> BackSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float BackSoundAdjuster = 1.f;

	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> ConfirmSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float ConfirmSoundAdjuster = 1.f;

#pragma endregion

#pragma region UI Curtain

	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> StartGameSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float StartGameSoundAdjuster = 1.f;

	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> TransitionUIInSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float TransitionUIInSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> TransitionUIOutSound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float TransitionUIOutSoundAdjuster = 1.f;

	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	TObjectPtr<UMetaSoundSource> TransitionUISound;
	UPROPERTY(EditAnywhere, Category = "Sounds UI")
	float TransitionUISoundAdjuster = 1.f;

#pragma endregion 
	
	
#pragma endregion
	
#pragma region Events Sounds

#pragma region DuckSounds
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckCashOutSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	float DuckCashOutSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckPickSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	float DuckPickSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> DuckStealSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	float DuckStealSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	TObjectPtr<UMetaSoundSource> GoldenDuckSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Duck")
	float GoldenDuckSoundAdjuster = 1.f;
#pragma endregion

#pragma region MoleSounds

	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	TObjectPtr<UMetaSoundSource> MoleImpactSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	float MoleImpactSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	TObjectPtr<UMetaSoundSource> MoleInSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	float MoleInSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	TObjectPtr<UMetaSoundSource> MoleOutSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Mole")
	float MoleOutSoundAdjuster = 1.f;
#pragma endregion 

#pragma region Pillar Sounds

	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	TObjectPtr<UMetaSoundSource> PillarInSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	float PillarInSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	TObjectPtr<UMetaSoundSource> PillarOutSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	float PillarOutSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	TObjectPtr<UMetaSoundSource> PillarGainSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	float PillarGainSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	TObjectPtr<UMetaSoundSource> PillarBaitSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Pillar")
	float PillarBaitSoundAdjuster = 1.f;
	
#pragma endregion 
	
#pragma endregion

	
#pragma region GrapplingSounds

	UPROPERTY(EditAnywhere, Category = "Sounds Grappling")
	TObjectPtr<UMetaSoundSource> GrapplingGrabSound;
	UPROPERTY(EditAnywhere, Category = "Sounds Grappling")
	float GrapplingGrabSoundAdjuster = 1.f;
	
#pragma endregion

#pragma region Players

	UPROPERTY(EditAnywhere, Category = "Player Respawn")
	TObjectPtr<UMetaSoundSource> PlayerRespawn;
	UPROPERTY(EditAnywhere, Category = "Player Respawn")
	float PlayerRespawnSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Player Impact")
	TObjectPtr<UMetaSoundSource> PlayerImpacts;
	UPROPERTY(EditAnywhere, Category = "Player Impact")
	float PlayerImpactsSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Player Punch")
	TObjectPtr<UMetaSoundSource> PlayerPunch;
	UPROPERTY(EditAnywhere, Category = "Player Punch")
	float PlayerPunchSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Player Death")
	TObjectPtr<UMetaSoundSource> PlayerDeath;
	UPROPERTY(EditAnywhere, Category = "Player Death")
	float PlayerDeathSoundAdjuster = 1.f;
	
#pragma endregion

#pragma region Power Up

	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	TObjectPtr<UMetaSoundSource> PowerUpFreezeCast;
	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	float PowerUpFreezeCastSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	TObjectPtr<UMetaSoundSource> PowerUpFreezePlayerTouched;
	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	float PowerUpFreezePlayerTouchedSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	TObjectPtr<UMetaSoundSource> PowerUpFreezeEffectFinished;
	UPROPERTY(EditAnywhere, Category = "Power Up Freeze")
	float PowerUpFreezeEffectFinishedSoundAdjuster = 1.f;
	
	
	UPROPERTY(EditAnywhere, Category = "Power Up Strength")
	TObjectPtr<UMetaSoundSource> PowerUpStrengthCast;
	UPROPERTY(EditAnywhere, Category = "Power Up Strength")
	float PowerUpStrengthCastSoundAdjuster = 1.f;
	
	
	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	TObjectPtr<UMetaSoundSource> PowerUpSlipperyCast;
	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	float PowerUpSlipperyCastSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	TObjectPtr<UMetaSoundSource> PowerUpSlipperyDisapear;
	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	float PowerUpSlipperyDisapearSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	TObjectPtr<UMetaSoundSource> PowerUpSlipperyEffect;
	UPROPERTY(EditAnywhere, Category = "Power Up Slippery")
	float PowerUpSlipperyEffectSoundAdjuster = 1.f;
	
	
#pragma endregion 

#pragma region Element Reactions
	
	UPROPERTY(EditAnywhere, Category = "Element Bumper")
	TObjectPtr<UMetaSoundSource> BumperImpact;
	UPROPERTY(EditAnywhere, Category = "Element Bumper")
	float BumperImpactSoundAdjuster = 1.f;
	

	UPROPERTY(EditAnywhere, Category = "Element Rail")
	TObjectPtr<UMetaSoundSource> RailEnter;
	UPROPERTY(EditAnywhere, Category = "Element Rail")
	float RailEnterSoundAdjuster = 1.f;
	
	UPROPERTY(EditAnywhere, Category = "Element Rail")
	TObjectPtr<UMetaSoundSource> RailExit;
	UPROPERTY(EditAnywhere, Category = "Element Rail")
	float RailExitSoundAdjuster = 1.f;
#pragma endregion 
	
};
