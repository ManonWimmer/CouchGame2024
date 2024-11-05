// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/Behaviors/PlayerBallBehaviorElementReactions.h"


UPlayerBallBehaviorElementReactions::UPlayerBallBehaviorElementReactions()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UPlayerBallBehaviorElementReactions::BeginPlay()
{
	Super::BeginPlay();

	
}


void UPlayerBallBehaviorElementReactions::TickComponent(float DeltaTime, ELevelTick TickType,
                                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerBallBehaviorElementReactions::InitBehavior()
{
	Super::InitBehavior();
}

void UPlayerBallBehaviorElementReactions::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	Super::BindBehaviorEventAction(InPlayerBallController);

	
}

