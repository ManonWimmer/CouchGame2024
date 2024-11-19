// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventDuck.h"

#include "AsyncTreeDifferences.h"


// Sets default values
AEventDuck::AEventDuck()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEventDuck::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventDuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEventDuck::SetupEventPhase1()
{
	Super::SetupEventPhase1();
}

void AEventDuck::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();
}

void AEventDuck::TriggerEventPhase2()
{
	Super::TriggerEventPhase2();
}

void AEventDuck::TickPhase1()
{
	Super::TickPhase1();
}

void AEventDuck::TickPhase2()
{
	Super::TickPhase2();
}

void AEventDuck::EndEvent()
{
	Super::EndEvent();
}


