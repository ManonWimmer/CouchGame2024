// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/PowerUp.h"

#include "Components/SphereComponent.h"
#include "Rounds/RoundsSubsystem.h"


APowerUp::APowerUp()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SphereTrigger"));
	PowerUpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerUpMesh"));

	RootComponent = SphereTrigger;
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
	ReceiveTriggerPowerUp();
	OnPowerUpCollected.Broadcast();
	this->Destroy();
}

void APowerUp::TriggerPowerUp(int Value)
{
	ReceiveTriggerPowerUp();
	OnPowerUpCollected.Broadcast();
	this->Destroy();
}

void APowerUp::InitResetable()
{
	if (!GetWorld())	return;

	URoundsSubsystem* RoundsSubsystem = GetWorld()->GetSubsystem<URoundsSubsystem>();

	if (RoundsSubsystem == nullptr)	return;

	RoundsSubsystem->AddResetableObject(this);
}

bool APowerUp::IsResetable()
{
	return true;
}

void APowerUp::ResetObject()
{
	TriggerPowerUp();
}

