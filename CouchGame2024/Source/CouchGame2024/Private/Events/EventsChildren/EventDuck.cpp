// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventDuck.h"

#include "AsyncTreeDifferences.h"
#include "Events/Duck/DuckSpawner.h"
#include "Kismet/GameplayStatics.h"


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

	TArray<AActor*> Acteurs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADuckSpawner::StaticClass(), Acteurs);

	for (AActor* Acteur : Acteurs)
	{
		ADuckSpawner* DuckSpawner = Cast<ADuckSpawner>(Acteur);
		if (DuckSpawner)
			DuckSpawner->BindDuckStartEvent();
	}
}

// Called every frame
void AEventDuck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEventDuck::SetupEventPhase1()
{
	Super::SetupEventPhase1();

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,"BROADCAST DUCK START");
	
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

	OnDuckEndedEvent.Broadcast();
}


