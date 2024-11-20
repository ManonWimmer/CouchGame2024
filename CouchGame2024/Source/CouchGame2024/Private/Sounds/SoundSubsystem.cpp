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

void USoundSubsystem::PlayGrapplingGrabSound()
{
	if (SoundsData == nullptr)	return;
	if (SoundsData->GrapplingGrabSound == nullptr)	return;

	if (GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundsData->GrapplingGrabSound);
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
