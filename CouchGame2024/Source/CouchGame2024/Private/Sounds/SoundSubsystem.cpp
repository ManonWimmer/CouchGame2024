// Fill out your copyright notice in the Description page of Project Settings.


#include "Sounds/SoundSubsystem.h"

#include "FCTween.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sounds/SoundsData.h"

#include "Sounds/SoundsSettings.h"


void USoundSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	InitSoundSubsystem();

	InitMusicAudioComponent();
}

void USoundSubsystem::InitMusicAudioComponent()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->MainMenuMusic == nullptr)	return;
	if (GetWorld())
	{
		MusicAudioComponent = UGameplayStatics::SpawnSound2D(GetWorld(), SoundsData->MainMenuMusic, 1.f, 1.f, 0.f, nullptr, false, false);
		MusicAudioComponent->SetUISound(true);
		MusicAudioComponent->Stop();
	}
}

void USoundSubsystem::FadeInMusic(UMetaSoundSource* InSound)
{
	if (InSound == nullptr)	return;
	if (MusicAudioComponent == nullptr)	return;

	MusicAudioComponent->SetSound(InSound);

	MusicAudioComponent->Play();
	MusicAudioComponent->FadeIn(1.5f, SoundsData->InGameMusicAdjuster, 0.f);
}

void USoundSubsystem::FadeOutMusic()
{
	MusicAudioComponent->FadeOut(1.5f, 0.f);
}


void USoundSubsystem::PlayInGameMusicSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->InGameMusic == nullptr)	return;
	
	if (MusicAudioComponent == nullptr)	return;

	FadeOutMusic();
	if (TweenMusic != nullptr)
	{
		TweenMusic->Destroy();
	}
	TweenMusic = FCTween::Play(0.f, 1.5f, [&](float Value){ int i = Value; }, 1.5f);
	TweenMusic->SetOnComplete([&] { FadeInMusic(SoundsData->InGameMusic); } );
}

void USoundSubsystem::PlayInGameDuckMusicSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->InGameDuckMusic == nullptr)	return;
	
	if (MusicAudioComponent == nullptr)	return;

	FadeOutMusic();
	if (TweenMusic != nullptr)
	{
		TweenMusic->Destroy();
	}
	TweenMusic = FCTween::Play(0.f, 1.5f, [&](float Value){ int i = Value; }, 1.5f);
	TweenMusic->SetOnComplete([&] { FadeInMusic(SoundsData->InGameDuckMusic); } );
}

void USoundSubsystem::PlayMainMenuMusicSound()
{
	
	if (SoundsData == nullptr)	return;
	if (SoundsData->MainMenuMusic == nullptr)	return;
	
	if (MusicAudioComponent == nullptr)	return;

	FadeOutMusic();
	if (TweenMusic != nullptr)
	{
		TweenMusic->Destroy();
	}
	TweenMusic = FCTween::Play(0.f, 1.5f, [&](float Value){ int i = Value; }, 1.5f);
	TweenMusic->SetOnComplete([&] { FadeInMusic(SoundsData->MainMenuMusic); } );
}

void USoundSubsystem::PlaySettingsMusicSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->SettingsMusic == nullptr)	return;
	
	if (MusicAudioComponent == nullptr)	return;

	FadeOutMusic();
	if (TweenMusic != nullptr)
	{
		TweenMusic->Destroy();
	}
	TweenMusic = FCTween::Play(0.f, 1.5f, [&](float Value){ int i = Value; }, 1.5f);
	TweenMusic->SetOnComplete([&] { FadeInMusic(SoundsData->SettingsMusic); } );
}

void USoundSubsystem::PlayWaitingMusicSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->WaitingMusic == nullptr)	return;
	
	if (MusicAudioComponent == nullptr)	return;

	FadeOutMusic();
	if (TweenMusic != nullptr)
	{
		TweenMusic->Destroy();
	}
	TweenMusic = FCTween::Play(0.f, 1.5f, [&](float Value){ int i = Value; }, 1.5f);
	TweenMusic->SetOnComplete([&] { FadeInMusic(SoundsData->WaitingMusic); } );
}

void USoundSubsystem::Play321MusicSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->InGame321Music == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->InGame321Music, SoundsData->InGame321MusicAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayRoundStartSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RoundStartSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RoundStartSound, SoundsData->RoundStartSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayCrowdEffectSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->CrowdEffectSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->CrowdEffectSound, SoundsData->CrowdEffectSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayUIButtonPressSound()
{
	if (SoundsData == nullptr)	return;
    if (SoundsData->ButtonPressSound == nullptr)	return;
    
    if (GetWorld())
    {
    	UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->ButtonPressSound, SoundsData->ButtonPressSoundAdjuster);
    }
}

void USoundSubsystem::PlayUICheckSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->CheckSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->CheckSound, SoundsData->CheckSoundAdjuster);
	}
}

void USoundSubsystem::PlayUIHoverSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->HoverSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->HoverSound, SoundsData->HoverSoundAdjuster);
	}
}

void USoundSubsystem::PlayUIBackSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->BackSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->BackSound, SoundsData->BackSoundAdjuster);
	}
}

void USoundSubsystem::PlayUIConfirmSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->ConfirmSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->ConfirmSound, SoundsData->ConfirmSoundAdjuster);
	}
}

void USoundSubsystem::PlayUIStartGameSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->StartGameSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->StartGameSound, SoundsData->StartGameSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayUITransitionInSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->TransitionUIInSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->TransitionUIInSound, SoundsData->TransitionUIInSoundAdjuster);
	}
}

void USoundSubsystem::PlayUITransitionOutSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->TransitionUIOutSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->TransitionUIOutSound, SoundsData->TransitionUIOutSoundAdjuster);
	}
}

void USoundSubsystem::PlayUITransitionSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->TransitionUISound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->TransitionUISound, SoundsData->TransitionUISoundAdjuster);
	}
}

void USoundSubsystem::PlayDuckCashOutSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckCashOutSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckCashOutSound, SoundsData->DuckCashOutSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayDuckPickSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckPickSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckPickSound, SoundsData->DuckPickSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayDuckStealSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckStealSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckStealSound, SoundsData->DuckStealSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayGoldenDuckPickSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->GoldenDuckSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->GoldenDuckSound, SoundsData->GoldenDuckSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayMoleImpactSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->MoleImpactSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->MoleImpactSound, SoundsData->MoleImpactSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayMoleInSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->MoleInSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->MoleInSound, SoundsData->MoleInSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayMoleOutSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->MoleOutSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->MoleOutSound, SoundsData->MoleOutSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPillarInSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PillarInSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PillarInSound, SoundsData->PillarInSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPillarOutSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PillarOutSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PillarOutSound, SoundsData->PillarOutSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPillarGainSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PillarGainSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PillarGainSound, SoundsData->PillarGainSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPillarBaitSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PillarBaitSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PillarBaitSound, SoundsData->PillarBaitSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayGrapplingGrabSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->GrapplingGrabSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->GrapplingGrabSound, SoundsData->GrapplingGrabSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPlayerRespawnSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerRespawn == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerRespawn, SoundsData->PlayerRespawnSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPlayerImpactsSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerImpacts == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerImpacts, SoundsData->PlayerImpactsSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPlayerPunchSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerPunch == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerPunch, SoundsData->PlayerPunchSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPlayerDeathSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerDeath == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerDeath, SoundsData->PlayerDeathSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayFreezeCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezeCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezeCast, SoundsData->PowerUpFreezeCastSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPlayerFreezedSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezePlayerTouched == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezePlayerTouched, SoundsData->PowerUpFreezePlayerTouchedSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayPlayerUnfreezeSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezeEffectFinished == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezeEffectFinished, SoundsData->PowerUpFreezeEffectFinishedSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayStrengthCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpStrengthCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpStrengthCast, SoundsData->PowerUpStrengthCastSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayStrengthDisapearSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpStrengthDisapear == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpStrengthDisapear, SoundsData->PowerUpStrengthDisapearAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayStrengthImpactSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpStrengthImpact == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpStrengthImpact, SoundsData->PowerUpStrengthImpactAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayStrengthEffectSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpStrengthEffect == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpStrengthEffect, SoundsData->PowerUpStrengthEffectAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlaySlipperyCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyCast, SoundsData->PowerUpSlipperyCastSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlaySlipperySlipperyDisapearSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyDisapear == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyDisapear, SoundsData->PowerUpSlipperyDisapearSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlaySlipperyEffectSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyEffect == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyEffect, SoundsData->PowerUpSlipperyEffectSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayBumperImpactSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->BumperImpact == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->BumperImpact, SoundsData->BumperImpactSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayRailEnterSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RailEnter == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RailEnter, SoundsData->RailEnterSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::PlayRailExitSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RailExit == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RailExit, SoundsData->RailExitSoundAdjuster, 1.f, 0.f, nullptr, nullptr, false);
	}
}

void USoundSubsystem::InitSoundSubsystem()
{
	const USoundsSettings* SoundsSettings = GetDefault<USoundsSettings>();

	if (SoundsSettings == nullptr)	return;
	
	USoundsData* OutSoundsData = SoundsSettings->SoundsData.LoadSynchronous();
	
	if (OutSoundsData == nullptr)	return;

	SoundsData = OutSoundsData;
}
