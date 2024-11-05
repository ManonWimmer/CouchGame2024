// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorGrapple.h"


UPlayerBallBehaviorGrapple::UPlayerBallBehaviorGrapple()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerBallBehaviorGrapple::BeginPlay()
{
	Super::BeginPlay();

	
}


void UPlayerBallBehaviorGrapple::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerBallBehaviorGrapple::InitBehavior()
{
	Super::InitBehavior();
}

void UPlayerBallBehaviorGrapple::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	
}


