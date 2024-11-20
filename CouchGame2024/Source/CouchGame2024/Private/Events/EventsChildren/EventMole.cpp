// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventMole.h"


// Sets default values
AEventMole::AEventMole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEventMole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventMole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEventMole::SetupEventPhase1()
{
	Super::SetupEventPhase1();
}

void AEventMole::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();

	OnMoleStartedEvent.Broadcast();
}

void AEventMole::TriggerEventPhase2()
{
	Super::TriggerEventPhase2();
}

void AEventMole::TickPhase1()
{
	Super::TickPhase1();
}

void AEventMole::TickPhase2()
{
	Super::TickPhase2();
}

void AEventMole::EndEvent()
{
	Super::EndEvent();

	OnMoleEndedEvent.Broadcast();
}

