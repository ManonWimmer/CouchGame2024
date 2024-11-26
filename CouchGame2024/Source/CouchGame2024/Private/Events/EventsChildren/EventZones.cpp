// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventZones.h"

#include "Events/Mole/MoleSpawner.h"
#include "Kismet/GameplayStatics.h"
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

}

// Called every frame
void AEventZones::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void AEventZones::SetupEventPhase1()
{
	Super::SetupEventPhase1();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APillarZone::StaticClass(), Actors);
    
	for (AActor* Actor : Actors)
	{
		if (APillarZone* PillarZone = Cast<APillarZone>(Actor))
			PillarZone->Bind();
	}
	
	OnZonesStartedEvent.Broadcast();
}

void AEventZones::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();
	
	OnZonesPhase1StartedEvent.Broadcast();
}

void AEventZones::TriggerEventPhase2()
{
	Super::TriggerEventPhase2();

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

