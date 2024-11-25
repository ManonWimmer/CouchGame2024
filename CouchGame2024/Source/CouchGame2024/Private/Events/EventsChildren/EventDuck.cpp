﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventDuck.h"

#include "Events/Duck/DuckBank.h"
#include "Events/Duck/DuckSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "PowerUp/Type/PowerUpDuck.h"


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

	TArray<AActor*> Acteurs;
    	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADuckSpawner::StaticClass(), Acteurs);
    
    	for (AActor* Acteur : Acteurs)
    	{
    		ADuckSpawner* DuckSpawner = Cast<ADuckSpawner>(Acteur);
    		if (DuckSpawner)
    			DuckSpawner->BindDuckStartAndEndEvent();
    	}
	
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,"BROADCAST DUCK START");

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADuckBank::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ADuckBank* DuckBank = Cast<ADuckBank>(Actor))
		{
			DuckBank->Bind();
		}
	}
	
	OnDuckStartedEvent.Broadcast();
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

	/*
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan,"Destroy ducks");

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (APowerUpDuck* DuckActor = Cast<APowerUpDuck>(Actor))
		{
			DuckActor->Destroy();
		}
	}
	*/

	OnDuckEndedEvent.Broadcast();
}


