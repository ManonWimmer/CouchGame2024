// Fill out your copyright notice in the Description page of Project Settings.


#include "PinballElements/Elements/FlipperElement.h"


// Sets default values
AFlipperElement::AFlipperElement()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFlipperElement::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlipperElement::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

