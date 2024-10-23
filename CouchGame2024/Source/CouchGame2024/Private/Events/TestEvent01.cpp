// Fill out your copyright notice in the Description page of Project Settings.


#include "Events/TestEvent01.h"


// Sets default values
ATestEvent01::ATestEvent01()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EventName = "TestEvent01";
}

// Called when the game starts or when spawned
void ATestEvent01::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEvent01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestEvent01::TriggerEvent()
{
	Super::TriggerEvent();
}

