// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SoundSubsystem.generated.h"

class USoundsData;
/**
 * 
 */
UCLASS()
class COUCHGAME2024_API USoundSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

virtual void OnWorldBeginPlay(UWorld& InWorld) override;

public:

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

#pragma endregion
	
private:
	UFUNCTION()
	void InitSoundSubsystem();


	UPROPERTY()
	TObjectPtr<USoundsData> SoundsData;
	
};
