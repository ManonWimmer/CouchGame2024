// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SoundSubsystem.generated.h"

class UMetaSoundSource;
class USoundsData;
class FCTweenInstance;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API USoundSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	virtual void Deinitialize() override;
	

public:

#pragma region Play Music

	UPROPERTY()
	TObjectPtr<UAudioComponent> MusicAudioComponent;

	FCTweenInstance* TweenMusic;
	
	UFUNCTION(BlueprintCallable)
	void InitMusicAudioComponent();

	UFUNCTION()
	void FadeInMusic(UMetaSoundSource* InSound, float InTimeStart = 0.f);

	UFUNCTION(BlueprintCallable)
	void FadeOutMusic();
	
	UFUNCTION(BlueprintCallable)
	void PlayInGameMusicSound();

#pragma region Events Musics

	UFUNCTION(BlueprintCallable)
	void PlayEventMusicSound(EEventName InEventName);
	
	UFUNCTION(BlueprintCallable)
	void PlayInGameDuckMusicSound();

#pragma endregion
	
	UFUNCTION(BlueprintCallable)
	void PlayMainMenuMusicSound();

	UFUNCTION(BlueprintCallable)
	void PlaySettingsMusicSound();

	UFUNCTION(BlueprintCallable)
	void UnplaySettingsMusicSound();

	UFUNCTION(BlueprintCallable)
	void PlayWaitingMusicSound();
	
	UFUNCTION(BlueprintCallable)
	void Play321MusicSound();

#pragma endregion

#pragma region Play Round Sounds

	UFUNCTION(BlueprintCallable)
	void PlayRoundStartSound();

	#pragma region Play Crowd Sounds

		UFUNCTION(BlueprintCallable)
		void PlayCrowdEffectSound();

	#pragma endregion 
#pragma endregion

#pragma region Play UI Sounds

	UFUNCTION(BlueprintCallable)
	void PlayUIButtonPressSound();

	UFUNCTION(BlueprintCallable)
	void PlayUICheckSound();

	UFUNCTION(BlueprintCallable)
	void PlayUIHoverSound();

	UFUNCTION(BlueprintCallable)
	void PlayUIBackSound();

	UFUNCTION(BlueprintCallable)
	void PlayUIConfirmSound();


	UFUNCTION(BlueprintCallable)
	void PlayUIStartGameSound();
	
	UFUNCTION(BlueprintCallable)
	void PlayUITransitionInSound();

	UFUNCTION(BlueprintCallable)
	void PlayUITransitionOutSound();

	UFUNCTION(BlueprintCallable)
	void PlayUITransitionSound();
	
#pragma endregion 

	
#pragma region Play Events Sounds
	
#pragma region Play Duck sounds
	UFUNCTION(BlueprintCallable)
	void PlayDuckCashOutSound();
	
	UFUNCTION(BlueprintCallable)
	void PlayDuckPickSound();

	UFUNCTION(BlueprintCallable)
	void PlayDuckStealSound();

	UFUNCTION(BlueprintCallable)
	void PlayGoldenDuckPickSound();

#pragma endregion

#pragma region Play Mole Sounds

	UFUNCTION(BlueprintCallable)
	void PlayMoleImpactSound();

	UFUNCTION(BlueprintCallable)
	void PlayMoleInSound();

	UFUNCTION(BlueprintCallable)
	void PlayMoleOutSound();

#pragma endregion 

#pragma region Play Pillar Sounds

	UFUNCTION(BlueprintCallable)
	void PlayPillarInSound();

	UFUNCTION(BlueprintCallable)
	void PlayPillarOutSound();

	UFUNCTION(BlueprintCallable)
	void PlayPillarGainSound();

	UFUNCTION(BlueprintCallable)
	void PlayPillarBaitSound();

#pragma endregion 
	
#pragma endregion 
	
#pragma region Play Grappling Sounds

	UFUNCTION(BlueprintCallable)
	void PlayGrapplingGrabSound();


#pragma endregion


#pragma region Play Players Sounds

	UFUNCTION(BlueprintCallable)
	void PlayPlayerRespawnSound();

	UFUNCTION(BlueprintCallable)
	void PlayPlayerImpactsSound();

	UFUNCTION(BlueprintCallable)
	void PlayPlayerPunchSound();

	UFUNCTION(BlueprintCallable)
	void PlayPlayerDeathSound();
	
#pragma endregion

#pragma region Power Ups Sounds

	UFUNCTION(BlueprintCallable)
	void PlayFreezeCastSound();

	UFUNCTION(BlueprintCallable)
	void PlayPlayerFreezedSound();

	UFUNCTION(BlueprintCallable)
	void PlayPlayerUnfreezeSound();


	UFUNCTION(BlueprintCallable)
	void PlayStrengthCastSound();

	UFUNCTION(BlueprintCallable)
	void PlayStrengthDisapearSound();
	
	UFUNCTION(BlueprintCallable)
	void PlayStrengthImpactSound();

	UFUNCTION(BlueprintCallable)
	void PlayStrengthEffectSound();
	
	
	UFUNCTION(BlueprintCallable)
	void PlaySlipperyCastSound();

	UFUNCTION(BlueprintCallable)
	void PlaySlipperySlipperyDisapearSound();

	UFUNCTION(BlueprintCallable)
	void PlaySlipperyEffectSound();

#pragma endregion

#pragma region Element reactions Sounds
	
#pragma region Bumper Sounds

	UFUNCTION(BlueprintCallable)
	void PlayBumperImpactSound();

#pragma endregion 

#pragma region Rail Sounds

	UFUNCTION(BlueprintCallable)
	void PlayRailEnterSound();

	UFUNCTION(BlueprintCallable)
	void PlayRailExitSound();

#pragma endregion 
	
#pragma endregion 

#pragma region Fireworks Sounds

	UFUNCTION(BlueprintCallable)
	void PlayFireworksSound();

#pragma endregion

	
	UFUNCTION(BlueprintCallable)
	void InitSoundSubsystem();


private:
	UPROPERTY()
	TObjectPtr<USoundsData> SoundsData;

	UPROPERTY()
	TObjectPtr<UMetaSoundSource> SoundToPlayAfterPause;

	float MusicPlayingDurationBeforePause = 0.f;
	
};
