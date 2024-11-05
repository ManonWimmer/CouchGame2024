// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorPowerUp.h"


UPlayerBallBehaviorPowerUp::UPlayerBallBehaviorPowerUp()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerBallBehaviorPowerUp::BeginPlay()
{
	Super::BeginPlay();

	
}


void UPlayerBallBehaviorPowerUp::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
}

void UPlayerBallBehaviorPowerUp::InitBehavior()
{
	Super::InitBehavior();
}

void UPlayerBallBehaviorPowerUp::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	
}

