// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/PowerUp.h"

#include "Components/SphereComponent.h"


APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTrigger"));
	PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerUpMesh"));

	SphereTrigger->SetupAttachment(RootComponent);
	PowerUpMesh->SetupAttachment(SphereTrigger);
}

void APowerUp::BeginPlay()
{
	Super::BeginPlay();
	
}

void APowerUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

EPowerUpID APowerUp::GetPowerUpID() const
{
	return EPowerUpID::None;
}

void APowerUp::TriggerPowerUp()
{
	this->Destroy();
}

