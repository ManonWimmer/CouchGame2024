// Fill out your copyright notice in the Description page of Project Settings.


#include "Fireworks/FireworkElement.h"


// Sets default values
AFireworkElement::AFireworkElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFireworkElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFireworkElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireworkElement::FireFirework()
{
	ReceiveOnFireFirework();
}

void AFireworkElement::StopFirework()
{
	
}

