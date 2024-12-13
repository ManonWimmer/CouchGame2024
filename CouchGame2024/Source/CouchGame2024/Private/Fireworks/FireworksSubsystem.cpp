// Fill out your copyright notice in the Description page of Project Settings.


#include "CouchGame2024/Public/Fireworks/FireworksSubsystem.h"

#include "Fireworks/FireworkElement.h"
#include "Kismet/GameplayStatics.h"

void UFireworksSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	InitFireworkSubsystem();
}

void UFireworksSubsystem::InitFireworkSubsystem()
{
	if (GetWorld() == nullptr)	return;

	TArray<AActor*> ActorsFireworks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFireworkElement::StaticClass(), ActorsFireworks);

	for (AActor* ActorFirework : ActorsFireworks)
	{
		if (ActorFirework == nullptr)	continue;

		AFireworkElement* Firework = Cast<AFireworkElement>(ActorFirework);
		
		if (Firework == nullptr)	continue;
		
		FireworkElements.Add(Firework);
	}
}

void UFireworksSubsystem::FireAllFireworks()
{
	for (AFireworkElement* Firework : FireworkElements)
	{
		if (Firework == nullptr)	continue;

		Firework->FireFirework();
	}
}

void UFireworksSubsystem::StopAllFireworks()
{
	for (AFireworkElement* Firework : FireworkElements)
	{
		if (Firework == nullptr)	continue;

		Firework->StopFirework();
	}
}
