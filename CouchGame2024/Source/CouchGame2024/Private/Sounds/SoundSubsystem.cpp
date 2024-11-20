// Fill out your copyright notice in the Description page of Project Settings.


#include "Sounds/SoundSubsystem.h"

void USoundSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	InitSoundSubsystem();
}

void USoundSubsystem::InitSoundSubsystem()
{
	
}
