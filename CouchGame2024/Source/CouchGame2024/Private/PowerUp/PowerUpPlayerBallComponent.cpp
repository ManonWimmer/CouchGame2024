// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUp/PowerUpPlayerBallComponent.h"


// Sets default values for this component's properties
UPowerUpPlayerBallComponent::UPowerUpPlayerBallComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPowerUpPlayerBallComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPowerUpPlayerBallComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPowerUpPlayerBallComponent::ReceiveFreezePowerUpAction()
{
	
}

void UPowerUpPlayerBallComponent::ReceiveStrengthPowerUpAction()
{
	
}

void UPowerUpPlayerBallComponent::ReceiveHeavyPowerUpAction()
{
	
}

