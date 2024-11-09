// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/PillarElement.h"


// Sets default values
APillarElement::APillarElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APillarElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APillarElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

