// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBall/PlayerBallBehaviorComponent.h"

#include "PlayerBall/PlayerBall.h"


// Sets default values for this component's properties
UPlayerBallBehaviorComponent::UPlayerBallBehaviorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerBallBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerBallBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerBallBehaviorComponent::InitBehavior()
{
	if (GetOwner())
	{
		PlayerBall = Cast<APlayerBall>(GetOwner());
	}

	SetupData();
}

void UPlayerBallBehaviorComponent::BindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	PlayerBallController = InPlayerBallController;
}

void UPlayerBallBehaviorComponent::UnbindBehaviorEventAction(APlayerBallController* InPlayerBallController)
{
	
}

void UPlayerBallBehaviorComponent::SetupData()
{
	
}


APlayerBall* UPlayerBallBehaviorComponent::GetPlayerBall() const
{
	return PlayerBall;
}

APlayerBallController* UPlayerBallBehaviorComponent::GetPlayerBallController() const
{
	return PlayerBallController;
}

