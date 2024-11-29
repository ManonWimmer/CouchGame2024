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

void USoundSubsystem::PlayDuckCashOutSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckCashOutSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckCashOutSound);
	}
}

void USoundSubsystem::PlayDuckPickSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckPickSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckPickSound);
	}
}

void USoundSubsystem::PlayDuckStealSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->DuckStealSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->DuckStealSound);
	}
}

void USoundSubsystem::PlayGoldenDuckPickSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->GoldenDuckSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->GoldenDuckSound);
	}
}

void USoundSubsystem::PlayGrapplingGrabSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->GrapplingGrabSound == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->GrapplingGrabSound);
	}
}

void USoundSubsystem::PlayPlayerRespawnSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerRespawn == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerRespawn);
	}
}

void USoundSubsystem::PlayPlayerImpactsSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerImpacts == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerImpacts);
	}
}

void USoundSubsystem::PlayPlayerPunchSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerPunch == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerPunch);
	}
}

void USoundSubsystem::PlayPlayerDeathSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PlayerDeath == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PlayerDeath);
	}
}

void USoundSubsystem::PlayFreezeCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezeCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezeCast);
	}
}

void USoundSubsystem::PlayPlayerFreezedSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezePlayerTouched == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezePlayerTouched);
	}
}

void USoundSubsystem::PlayPlayerUnfreezeSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpFreezeEffectFinished == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpFreezeEffectFinished);
	}
}

void USoundSubsystem::PlayStrengthCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpStrengthCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpStrengthCast);
	}
}

void USoundSubsystem::PlaySlipperyCastSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyCast == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyCast);
	}
}

void USoundSubsystem::PlaySlipperySlipperyDisapearSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyDisapear == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyDisapear);
	}
}

void USoundSubsystem::PlaySlipperyEffectSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->PowerUpSlipperyEffect == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->PowerUpSlipperyEffect);
	}
}

void USoundSubsystem::PlayBumperImpactSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->BumperImpact == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->BumperImpact);
	}
}

void USoundSubsystem::PlayRailEnterSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RailEnter == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RailEnter);
	}
}

void USoundSubsystem::PlayRailExitSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->RailExit == nullptr)	return;
	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->RailExit);
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
