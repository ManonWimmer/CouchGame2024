// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventZones.h"

#include "Kismet/GameplayStatics.h"
#include "Zone/PillarZone.h"
#include "Zone/PillarZone.h"


// Sets default values
AEventZones::AEventZones()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEventZones::BeginPlay()
{
	Super::BeginPlay();

	// Get scene Pillar Zones
	PillarZones.Reset();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillarZone::StaticClass(), Actors);
    
	for (AActor* Actor : Actors)
	{
		if (APillarZone* PillarZone = Cast<APillarZone>(Actor))
		{
			PillarZones.Add(PillarZone);
		}
	}
}

// Called every frame
void AEventZones::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEventZones::SetupEventPhase1()
{
	Super::SetupEventPhase1();
	
	for (TObjectPtr<APillarZone> PillarZone : PillarZones)
	{
		PillarZone->Bind();
		PillarZone->HideZone();
	}
	
	OnZonesStartedEvent.Broadcast();
}

void AEventZones::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();

	for (TObjectPtr<APillarZone> PillarZone : PillarZones)
	{
		if (PillarZone->PillarZonePhase ==  EPillarZonePhase::Phase1)
		{
			PillarZone->SetActorHiddenInGame(false);
			PillarZone->SetActorEnableCollision(true);
		}
		else
		{
			PillarZone->SetActorHiddenInGame(true);
			PillarZone->SetActorEnableCollision(false);
		}
	}
	
	OnZonesPhase1StartedEvent.Broadcast();
}

void AEventZones::TriggerEventPhase2()
{
	Super::TriggerEventPhase2();

	for (TObjectPtr<APillarZone> PillarZone : PillarZones)
	{
		if (PillarZone->PillarZonePhase ==  EPillarZonePhase::Phase2)
		{
			PillarZone->SetActorHiddenInGame(false);
			PillarZone->SetActorEnableCollision(true);
			
			PillarZone->Bind();
		}
		else
		{
			PillarZone->SetActorHiddenInGame(true);
			PillarZone->SetActorEnableCollision(false);
		}
	}

	OnZonesPhase2StartedEvent.Broadcast();
}

void AEventZones::TickPhase1()
{
	Super::TickPhase1();
}

void AEventZones::TickPhase2()
{
	Super::TickPhase2();
}

void AEventZones::EndEvent()
{
	Super::EndEvent();

	OnZonesEndedEvent.Broadcast();
}

