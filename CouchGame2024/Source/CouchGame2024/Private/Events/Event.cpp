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
}

