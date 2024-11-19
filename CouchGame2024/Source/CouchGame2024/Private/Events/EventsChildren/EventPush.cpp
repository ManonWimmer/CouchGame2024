// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/EventsChildren/EventPush.h"


// Sets default values
AEventPush::AEventPush()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEventPush::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEventPush::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

