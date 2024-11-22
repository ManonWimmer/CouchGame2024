// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventZones.h"

#include "Events/Mole/MoleSpawner.h"
#include "Kismet/GameplayStatics.h"


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

