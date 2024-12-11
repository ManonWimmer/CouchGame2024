// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventZones.h"

#include "Kismet/GameplayStatics.h"
#include "Sounds/SoundSubsystem.h"
#include "Zone/EventZonesV2Manager.h"
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

	// Get Phase 2 Manager
	TArray<AActor*> Actors2;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEventZonesV2Manager::StaticClass(), Actors2);
    
	if (Actors2.Num() > 0)
	{
		Phase2Manager = Cast<AEventZonesV2Manager>(Actors2[0]);
	}
	else
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1,15,FColor::Red,"EventZonesV2Manager could not be found");
		UE_LOG(LogTemp, Warning, TEXT("No actor of class AEventZonesV2Manager found"));
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
	UE_LOG(LogTemp, Warning, TEXT("SetupEventPhase1 zones"));
	
	for (TObjectPtr<APillarZone> PillarZone : PillarZones)
	{
		PillarZone->Bind();
		PillarZone->HideZone();
	}

	if (Phase2Manager != nullptr) Phase2Manager->Bind();
	UE_LOG(LogTemp, Warning, TEXT("Bind phase 2"));
	
	OnZonesStartedEvent.Broadcast();
}

void AEventZones::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();
	UE_LOG(LogTemp, Warning, TEXT("TriggerPhase1 zones"));

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