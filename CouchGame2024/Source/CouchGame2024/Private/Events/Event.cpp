// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/Event.h"


// Sets default values
AEvent::AEvent()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEvent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bInTickPhase1)
	{
		TickPhase1();
	}
	else if (bInTickPhase2)
	{
		TickPhase2();
	}
}

void AEvent::SetupEventPhase1() // show or hide objects
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
										 FString::Printf(
											 TEXT("Setup event phase 1 : %s"), *this->GetName()));

	bInTickPhase1 = false;
	bInTickPhase2 = false; 
}

void AEvent::TriggerEventPhase1() // start tick phase 1 
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
										 FString::Printf(
											 TEXT("Trigger event phase 1 : %s"), *this->GetName()));

	bInTickPhase1 = true;
	bInTickPhase2 = false;
}

void AEvent::TriggerEventPhase2() // start tick phase 2
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
										 FString::Printf(
											 TEXT("Trigger event phase 2 : %s"), *this->GetName()));

	bInTickPhase1 = false;
	bInTickPhase2 = true;
}

void AEvent::EndEvent() // stop tick
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow,
										 FString::Printf(
											 TEXT("End event : %s"), *this->GetName()));

	bInTickPhase1 = false;
	bInTickPhase2 = false;
}

void AEvent::TickPhase1()
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Tick Phase 1");
}

void AEvent::TickPhase2()
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "Tick Phase 2");
}

