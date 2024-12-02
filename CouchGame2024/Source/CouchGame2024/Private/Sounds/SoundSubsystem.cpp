// Fill out your copyright notice in the Description page of Project Settings.


#include "Sounds/SoundSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "Sounds/SoundsData.h"

#include "Sounds/SoundsSettings.h"

void USoundSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	InitSoundSubsystem();
}

void USoundSubsystem::PlayInGameMusicSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->InGameMusic == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->InGameMusic, SoundsData->InGameMusicAdjuster);
	}
}

void USoundSubsystem::PlayRoundStartSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RoundStartSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RoundStartSound, SoundsData->RoundStartSoundAdjuster);
	}
}

void USoundSubsystem::PlayCrowdEffectSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->CrowdEffectSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->CrowdEffectSound, SoundsData->CrowdEffectSoundAdjuster);
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

void USoundSubsystem::PlayUIStartGameSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->StartGameSound == nullptr)	return;
    
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->StartGameSound, SoundsData->StartGameSoundAdjuster);
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
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckCashOutSound, SoundsData->DuckCashOutSoundAdjuster);
	}
}

void USoundSubsystem::PlayDuckPickSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckPickSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckPickSound, SoundsData->DuckPickSoundAdjuster);
	}
}

void USoundSubsystem::PlayDuckStealSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckStealSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckStealSound, SoundsData->DuckStealSoundAdjuster);
	}
}

void USoundSubsystem::PlayGoldenDuckPickSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->GoldenDuckSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->GoldenDuckSound, SoundsData->GoldenDuckSoundAdjuster);
	}
}

void USoundSubsystem::PlayMoleImpactSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->MoleImpactSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->MoleImpactSound, SoundsData->MoleImpactSoundAdjuster);
	}
}

void USoundSubsystem::PlayMoleInSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->MoleInSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->MoleInSound, SoundsData->MoleInSoundAdjuster);
	}
}

void USoundSubsystem::PlayMoleOutSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->MoleOutSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->MoleOutSound, SoundsData->MoleOutSoundAdjuster);
	}
}

void USoundSubsystem::PlayGrapplingGrabSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->GrapplingGrabSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->GrapplingGrabSound, SoundsData->GrapplingGrabSoundAdjuster);
	}
}

void USoundSubsystem::PlayPlayerRespawnSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerRespawn == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerRespawn, SoundsData->PlayerRespawnSoundAdjuster);
	}
}

void USoundSubsystem::PlayPlayerImpactsSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerImpacts == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerImpacts, SoundsData->PlayerImpactsSoundAdjuster);
	}
}

void USoundSubsystem::PlayPlayerPunchSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerPunch == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerPunch, SoundsData->PlayerPunchSoundAdjuster);
	}
}

void USoundSubsystem::PlayPlayerDeathSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerDeath == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerDeath, SoundsData->PlayerDeathSoundAdjuster);
	}
}

void USoundSubsystem::PlayFreezeCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezeCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezeCast, SoundsData->PowerUpFreezeCastSoundAdjuster);
	}
}

void USoundSubsystem::PlayPlayerFreezedSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezePlayerTouched == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezePlayerTouched, SoundsData->PowerUpFreezePlayerTouchedSoundAdjuster);
	}
}

void USoundSubsystem::PlayPlayerUnfreezeSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezeEffectFinished == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezeEffectFinished, SoundsData->PowerUpFreezeEffectFinishedSoundAdjuster);
	}
}

void USoundSubsystem::PlayStrengthCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpStrengthCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpStrengthCast, SoundsData->PowerUpStrengthCastSoundAdjuster);
	}
}

void USoundSubsystem::PlaySlipperyCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyCast, SoundsData->PowerUpSlipperyCastSoundAdjuster);
	}
}

void USoundSubsystem::PlaySlipperySlipperyDisapearSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyDisapear == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyDisapear, SoundsData->PowerUpSlipperyDisapearSoundAdjuster);
	}
}

void USoundSubsystem::PlaySlipperyEffectSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyEffect == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyEffect, SoundsData->PowerUpSlipperyEffectSoundAdjuster);
	}
}

void USoundSubsystem::PlayBumperImpactSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->BumperImpact == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->BumperImpact, SoundsData->BumperImpactSoundAdjuster);
	}
}

void USoundSubsystem::PlayRailEnterSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RailEnter == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RailEnter, SoundsData->RailEnterSoundAdjuster);
	}
}

void USoundSubsystem::PlayRailExitSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RailExit == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RailExit, SoundsData->RailExitSoundAdjuster);
	}
}

void USoundSubsystem::InitSoundSubsystem()
{
	const USoundsSettings* SoundsSettings = GetDefault<USoundsSettings>();

	if (SoundsSettings == nullptr)	return;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Grappling Grab Sound test 0");

	USoundsData* OutSoundsData = SoundsSettings->SoundsData.LoadSynchronous();
	
	if (OutSoundsData == nullptr)	return;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Grappling Grab Sound test 1");

	SoundsData = OutSoundsData;
}
