// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventMole.h"

#include "Events/Mole/MoleSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "PinballElements/Elements/MoleElement.h"
#include "Sounds/SoundSubsystem.h"


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

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan, "LAAAAAA");

	TArray<AActor*> Acteurs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMoleSpawner::StaticClass(), Acteurs);

	for (AActor* Acteur : Acteurs)
	{
		AMoleSpawner* MoleSpawner = Cast<AMoleSpawner>(Acteur);
		if (MoleSpawner)
			MoleSpawner->Bind();
	}
}

void AEventMole::TriggerEventPhase1()
{
	Super::TriggerEventPhase1();

	OnMoleStartedEvent.Broadcast();

	OnMoleEventStarted();
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

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan, "end mole event");

	/*
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1,5,FColor::Cyan,"Destroy ducks");

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (AMoleElement* MoleActor = Cast<AMoleElement>(Actor))
		{
			MoleActor->Destroy();
		}
	}
	*/

	OnMoleEndedEvent.Broadcast();
}

void AEventMole::OnMoleEventStarted()
{
	if (GetGameInstance() == nullptr)	return;
	
	USoundSubsystem* SoundSubsystem = GetGameInstance()->GetSubsystem<USoundSubsystem>();

	if (SoundSubsystem == nullptr)	return;

	SoundSubsystem->PlayInGameDuckMusicSound();
}

